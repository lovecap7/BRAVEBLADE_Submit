#include "BossMuscleStateJumpAttack.h"
#include "BossMuscleStateIdle.h"
#include "BossMuscleStateDeath.h"
#include "BossMuscleStateHit.h"
#include "BossMuscle.h"
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
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../ActorManager.h"
#include "../../../Attack/WaveAttack.h"
#include "../../../../../General/Effect/EffekseerManager.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 180;
	//ディレイ1
	constexpr int kDelay1Frame = 40;
	//ディレイ2
	constexpr int kDelay2Frame = 80;

}

BossMuscleStateJumpAttack::BossMuscleStateJumpAttack(std::weak_ptr<Actor> owner, bool isAngry, const std::weak_ptr<ActorManager> actorManager):
	BossMuscleStateBase(owner, isAngry),
	m_attackCountFrame(0)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kJumpAttackName);
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//相手のほうを向く
	coll->LookAtTarget();
	//VC
	coll->CharacterVC("Attack");
}

BossMuscleStateJumpAttack::~BossMuscleStateJumpAttack()
{
	auto coolTime = kAttackCoolTime;
	//怒り状態ならクールタイムを短くする
	if (m_isAngry)coolTime *= 0.5f;
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetAttackCoolTime(coolTime);
	//グループに所属しているなら攻撃権を消す
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossMuscleStateJumpAttack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossMuscleStateJumpAttack::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BossMuscleStateDeath>(m_owner));
		return;
	}
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossMuscleStateHit>(m_owner, m_isAngry));
		return;
	}
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		//待機状態に戻す
		ChangeState(std::make_shared<BossMuscleStateIdle>(m_owner, m_isAngry));
		return;
	}
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		CreateAttack(actorManager);
	}
	//怒り状態の時
	if (m_isAngry)
	{
		if (m_attackCountFrame == m_attackData.startFrame + kDelay1Frame)
		{
			CreateAttack(actorManager);
		}
		if (m_attackCountFrame == m_attackData.startFrame + kDelay2Frame)
		{
			CreateAttack(actorManager);
		}
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossMuscleStateJumpAttack::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//作成と参照
	auto attack = std::dynamic_pointer_cast<WaveAttack>(actorManager.lock()->CreateAttack(AttackType::Wave, m_owner).lock());
	//攻撃を作成
	auto data = m_attackData;
	//大きさ
	attack->SetRadius(data.radius);
	//中点
	attack->SetPos(m_owner.lock()->GetPos());
	//波の広がる速度
	attack->SetWaveSpeed(data.moveSpeed);
	//ダメージ、持続フレーム、ノックバックの大きさ、攻撃の重さ、ヒットストップの長さ、カメラの揺れ
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
	//必殺エフェクト
	auto eff = EffekseerManager::GetInstance().CreateEffect("WaveAttackEff", m_owner.lock()->GetPos());
}

