#include "PlayerStateUltimate.h"
#include "PlayerStateIdle.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "UltGage.h"
#include "../../ActorManager.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/SphereCollider.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../General/Effect/MyEffect.h"
#include "../../Attack/ULT.h"
#include "../../../../Game/Camera/GameCamera/GameCamera.h"

namespace
{
	//武器の座標
	constexpr int kRightHandIndex = 43;
	//武器の長さ
	constexpr float kSwordHeight = 3000.0f;
	//攻撃判定をリセットする頻度
	constexpr int kUltResetFrame = 10;
	//アニメーションの速度の変化量
	constexpr float kAddAnimSpeed = 0.2f;
	//アニメーションの最高速度
	constexpr float kMaxAnimSpeed = 2.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//モデルの旋回速度
	constexpr int kModelRotateSpeed = 100;
}

PlayerStateUltimate::PlayerStateUltimate(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager) :
	PlayerStateBase(player),
	m_animCountFrame(0)
{
	//チャージフレームが持続フレームより大きいかを比較
	m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kULTName);
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//必殺技
	coll->SetCollState(CollisionState::Normal);
	auto model = coll->GetModel();
	m_animSpeed = m_attackData.animSpeed;
	model->SetAnim(m_attackData.anim.c_str(), false, m_animSpeed);
	//向きの更新
	Vector2 dir = coll->GetPlayerStickVec();
	model->SetDir(dir);
	//ゲージを0に
	coll->GetUltGage().lock()->ResetUltGage();
	coll->GetUltGage().lock()->SetPendingUltGage(0);
	//無敵
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
	//剣のエフェクト
	m_laserChargeEff = EffekseerManager::GetInstance().CreateEffect("UltChargeSwordEff", m_owner.lock()->GetPos());
	//モデルの旋回速度
	model->SetRotSpeed(kModelRotateSpeed);
	//SE
	coll->CharacterOnceSE("UltCharge");
	//VC
	coll->CharacterVC("Ult");
}


PlayerStateUltimate::~PlayerStateUltimate()
{
	//攻撃判定の削除
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	if (!m_attack.expired())m_attack.lock()->Delete();
	//エフェクトの削除
	if (!m_laserChargeEff.expired())m_laserChargeEff.lock()->Delete();
	if (!m_laserEff.expired())m_laserEff.lock()->Delete();
	//無敵
	coll->GetHitPoints().lock()->SetIsNoDamege(false);
}

void PlayerStateUltimate::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateUltimate::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	++m_animCountFrame;
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto model = coll->GetModel();
	//ボスを倒す
	if (actorManager.lock()->IsBossDisappear())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//ボスの体力がなくなった場合またはアニメーションが終了したら
	if (actorManager.lock()->IsBossDead() ||
		model->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//チャージエフェクトの位置更新
	if (!m_laserChargeEff.expired())
	{
		VECTOR rightHand = MV1GetFramePosition(model->GetModelHandle(), kRightHandIndex);
		m_laserChargeEff.lock()->SetPos(rightHand);
	}

	//攻撃発生フレーム
	if (m_animCountFrame == m_attackData.startFrame)
	{
		//SE
		coll->CharacterOnceSE("UltLaser");
		coll->CharacterOnceSE("UltShot");
		//攻撃作成
		CreateAttack(actorManager);
	}
	//攻撃判定をリセット
	if (m_animCountFrame % kUltResetFrame == 0)
	{
		if (!m_attack.expired())m_attack.lock()->ResetHitId();
	}
	//少しずつ減速する
	coll->GetRb()->SpeedDown(kMoveDeceRate);
	//向きの更新
	Vector2 dir = coll->GetPlayerStickVec();
	model->SetDir(dir);
	//レーザーの位置更新
	UpdateLaserPos();
}

void PlayerStateUltimate::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//作成と参照
	auto attack = std::dynamic_pointer_cast<ULT>(actorManager.lock()->CreateAttack(AttackType::ULT, m_owner).lock());
	//攻撃を作成
	auto data = m_attackData;
	//大きさ
	attack->SetRadius(data.radius);
	//ダメージ、持続フレーム、ノックバックの大きさ、攻撃の重さ、ヒットストップの長さ、カメラの揺れ
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
	//攻撃の参照を保存
	m_attack = attack;
	//必殺エフェクト
	auto eff = EffekseerManager::GetInstance().CreateEffect("UltLaserEff", m_owner.lock()->GetPos());
	//エフェクトの参照を保存
	m_laserEff = eff; 
	//攻撃の位置を更新
	UpdateLaserPos();
}

void PlayerStateUltimate::UpdateLaserPos()
{
	auto owner = m_owner.lock();
	//攻撃の位置
	Vector3 startPos = owner->GetPos();
	Vector3 endPos = owner->GetPos() + (owner->GetModel()->GetDir() * kSwordHeight);
	if(!m_attack.expired())
	{
		//攻撃の参照
		auto attack = m_attack.lock();
		//攻撃の位置を更新
		attack->SetStartPos(startPos);
		attack->SetEndPos(endPos);
	}
	if(!m_laserEff.expired())
	{	
		auto eff = m_laserEff.lock();
		//エフェクトの位置
		eff->SetPos(startPos);
		eff->LookAt(owner->GetModel()->GetDir());
	}
}

