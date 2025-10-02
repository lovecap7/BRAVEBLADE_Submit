#include "PlayerStateCA.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "../Enemy/EnemyBase.h"
#include "../../ActorManager.h"
#include "UltGage.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../General/Effect/TrackActorEffect.h"
#include "../../../../General/Effect/MyEffect.h"
#include "../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../Attack/Slash.h"

namespace
{
	//武器の座標と当たり判定の情報
	//右手の薬指のインデックス
	constexpr int kRightRingFingerIndex = 55;
	constexpr int kRightIndexFingerIndex = 43;
	//武器の長さ
	constexpr float kSwordHeight = 130.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//エフェクトの削除を遅らせる
	constexpr int kDeleteEffectDelayFrame = 20;
	//予備動作中の回転
	constexpr float kRotaAngle = 1.0f;
}

PlayerStateCA::PlayerStateCA(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager, AttackData data)	:
	PlayerStateBase(player),
	m_attackCountFrame(0),
	m_attackData(data)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//チャージ攻撃
	auto model = coll->GetModel();
	model->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//加算ゲージの予約
	coll->GetUltGage().lock()->SetPendingUltGage(m_attackData.addUltGage);
	//風エフェクト
	m_tornadeEff = EffekseerManager::GetInstance().CreateTrackActorEffect("CATornade", m_owner.lock());
	//1回転にかかるフレーム(持続 / 攻撃回数)
	m_oneRotaFrame = m_attackData.keepFrame / m_attackData.attackNum;
	//回転量(1回転 / 1回転にかかるフレーム)
	m_attackRotaAngle = -360.0f / static_cast<float>(m_oneRotaFrame);
	//斬撃エフェクトを段階に応じて作成
	if (data.attackName == kCA3Name)
	{
		//斬撃2エフェクト
		m_slashEff = EffekseerManager::GetInstance().CreateEffect("SlashtTraject2Eff", m_owner.lock()->GetPos());
	}
	else
	{
		//斬撃1エフェクト
		m_slashEff = EffekseerManager::GetInstance().CreateEffect("SlashtTraject1Eff", m_owner.lock()->GetPos());
	}
	//SE
	coll->CharacterOnceSE("CA");
	//VC
	RandAttackVC();
}

PlayerStateCA::~PlayerStateCA()
{
	//攻撃判定の削除
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	if (!m_attack.expired())m_attack.lock()->Delete();
	if (!m_slashEff.expired())m_slashEff.lock()->Delete();
	//エフェクトを数フレーム後削除
	if (!m_tornadeEff.expired())m_tornadeEff.lock()->SpecificFrame(kDeleteEffectDelayFrame);
}
void PlayerStateCA::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateCA::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto model = coll->GetModel();
	//ボスが完全に消滅したとき
	if (actorManager.lock()->IsBossDisappear())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//ボスの体力がなくなるか攻撃の持続が切れたら
	if (actorManager.lock()->IsBossDead() ||
		m_attackCountFrame > m_attackData.keepFrame)
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//死亡した
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_owner));
		return;
	}
	//攻撃を受けたなら
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_owner));
		return;
	}
	++m_attackCountFrame;
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		//攻撃発生
		CreateAttack(m_attackData.radius, m_attackData.damege, m_attackData.keepFrame,
			m_attackData.knockBackPower, m_attackData.attackWeight, actorManager);
	}
	//1回転ごとに攻撃判定のリセット
	if (m_attackCountFrame % m_oneRotaFrame == 0)
	{
		if (!m_attack.expired())m_attack.lock()->ResetHitId();
	}
	//回転
	if (!m_attack.expired())
	{
		model->SetRot(VGet(0.0f, m_attackRotaAngle, 0.0f));
	}
	//攻撃の位置更新
	if (!m_attack.expired())
	{
		UpdateAttackPos();
	}
	auto& input = Input::GetInstance();
	//入力があるなら
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//移動
		coll->GetRb()->SetMoveVec(GetForwardVec(camera) * m_attackData.moveSpeed);
	}
	else
	{
		//減速
		coll->GetRb()->SpeedDown(kMoveDeceRate);
	}
}

void PlayerStateCA::CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager)
{
	auto owner = m_owner.lock();
	//作成と参照
	m_attack = std::dynamic_pointer_cast<Slash>(actorManager.lock()->CreateAttack(AttackType::Slash, m_owner).lock());
	//攻撃を作成
	auto attack = m_attack.lock();
	auto data = m_attackData;
	//大きさ
	attack->SetRadius(data.radius);
	//ダメージ、持続フレーム、ノックバックの大きさ、攻撃の重さ、ヒットストップの長さ、カメラの揺れ
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
}

void PlayerStateCA::UpdateAttackPos()
{
	auto model = m_owner.lock()->GetModel();
	//武器
	//右手の薬指と人差し指の座標から武器の座標を出す
	VECTOR ringFinger = MV1GetFramePosition(model->GetModelHandle(), kRightRingFingerIndex);//薬指
	VECTOR indexFinger = MV1GetFramePosition(model->GetModelHandle(), kRightIndexFingerIndex);//人差し指
	//武器の矛先
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	VECTOR swordTip = VAdd(ringFinger, VScale(swordDir, kSwordHeight));//持ち手の座標に加算して剣先の座標を出す
	if (!m_attack.expired())
	{
		//座標をセット
		m_attack.lock()->SetStartPos(ringFinger);
		m_attack.lock()->SetEndPos(swordTip);
	}
	if (!m_slashEff.expired())
	{
		m_slashEff.lock()->SetPos(swordTip);
	}
}
