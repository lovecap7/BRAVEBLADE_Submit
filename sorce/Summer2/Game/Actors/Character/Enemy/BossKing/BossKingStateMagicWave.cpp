#include "BossKingStateMagicWave.h"
#include "BossKingStateIdle.h"
#include "BossKingStateHit.h"
#include "BossKingStateDeath.h"
#include "BossKingStateChase.h"
#include "BossKingStateChange.h"
#include "BossKing.h"
#include "../EnemyBase.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../General/Sound/SoundManager.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../ActorManager.h"
#include "../../../Attack/WaveAttack.h"
#include "../../../../../General/Effect/EffekseerManager.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 140;
	//右手と左手
	constexpr int kRightHandIndex = 41;
	constexpr int kLeftHandIndex = 17;
}

BossKingStateMagicWave::BossKingStateMagicWave(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager) :
	BossKingStateBase(owner, false),
	m_attackCountFrame(0)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kMagicWaveName);
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//相手のほうを向く
	coll->LookAtTarget();
	//VC
	RandAttackVC();
}

BossKingStateMagicWave::~BossKingStateMagicWave()
{
	auto coolTime = kAttackCoolTime;
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetAttackCoolTime(coolTime);
	//グループに所属しているなら攻撃権を消す
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossKingStateMagicWave::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossKingStateMagicWave::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//コライダブル
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		//第二形態なら
		if (m_isTransformSecond)
		{
			//死亡
			ChangeState(std::make_shared<BossKingStateDeath>(m_owner));
			return;
		}
		else
		{
			//変身
			ChangeState(std::make_shared<BossKingStateChange>(m_owner));
			return;
		}
	}
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossKingStateHit>(m_owner, m_isTransformSecond));
		return;
	}
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		//待機状態に戻す
		ChangeState(std::make_shared<BossKingStateIdle>(m_owner, false));
		return;
	}
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		//SE
		SoundManager::GetInstance().PlayOnceSE("Wave");
		auto model = coll->GetModel();
		//右手と左手から出す
		CreateAttack(actorManager, MV1GetFramePosition(model->GetModelHandle(), kRightHandIndex));
		CreateAttack(actorManager, MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex));
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossKingStateMagicWave::CreateAttack(const std::weak_ptr<ActorManager> actorManager, Vector3 pos)
{
	//作成と参照
	auto attack = std::dynamic_pointer_cast<WaveAttack>(actorManager.lock()->CreateAttack(AttackType::Wave, m_owner).lock());
	//攻撃を作成
	auto data = m_attackData;
	//大きさ
	attack->SetRadius(data.radius);
	//中点
	attack->SetPos(pos);
	//波の広がる速度
	attack->SetWaveSpeed(data.moveSpeed);
	//ダメージ、持続フレーム、ノックバックの大きさ、攻撃の重さ、ヒットストップの長さ、カメラの揺れ
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
	//必殺エフェクト
	auto eff = EffekseerManager::GetInstance().CreateEffect("MagicWaveEff", pos);
}

