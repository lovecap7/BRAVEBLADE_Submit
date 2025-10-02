#include "BossMuscleStateIdle.h"
#include "BossMuscleStateAngry.h"
#include "BossMuscleStateDeath.h"
#include "BossMuscleStateHit.h"
#include "BossMuscleStateRightPunch.h"
#include "BossMuscleStateBeam.h"
#include "BossMuscleStateJumpAttack.h"
#include "BossMuscleStateChase.h"
#include "BossMuscle.h"
#include "../EnemyBase.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../ActorManager.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーションの名前
	const char* kNormalAnim = "Boss|Idle1";//待機
	const char* kAnglyAnim = "Boss|Idle2";//待機
}

BossMuscleStateIdle::BossMuscleStateIdle(std::weak_ptr<Actor> owner, bool isAngry) :
	BossMuscleStateBase(owner,isAngry)
{
	//待機状態
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	if (isAngry)
	{
		//怒り状態の待機
		coll->GetModel()->SetAnim(kAnglyAnim, true);
	}
	else
	{
		//通常の待機
		coll->GetModel()->SetAnim(kNormalAnim, true);
	}
	coll->SetCollState(CollisionState::Normal);
}

BossMuscleStateIdle::~BossMuscleStateIdle()
{
}

void BossMuscleStateIdle::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossMuscleStateIdle::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
#if _DEBUG
	//ボスを怒らせる
	if (Input::GetInstance().IsTrigger("BossAngry"))
	{
		//怒り状態にする
		ChangeState(std::make_shared<BossMuscleStateAngry>(m_owner));
		return;
	}
#endif
	//コライダブル
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	auto targetData = coll->GetTargetData();
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BossMuscleStateDeath>(m_owner));
		return;
	}
	//体力が半分以下なら
	if (coll->GetHitPoints().lock()->GetHp() <= coll->GetHitPoints().lock()->GetMaxHp() / 2 &&
		!m_isAngry)
	{
		//怒り状態にする
		ChangeState(std::make_shared<BossMuscleStateAngry>(m_owner));
		return;
	}
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossMuscleStateHit>(m_owner,m_isAngry));
		return;
	}
	//プレイヤーを見つけたなら
	if (targetData.isHitTarget)
	{
		//プレイヤーを見る
		coll->LookAtTarget();
		//近づく距離
		if (targetData.targetDis > kMeleeAttackDistance)
		{
			//プレイヤーをに近づく
			ChangeState(std::make_shared<BossMuscleStateChase>(m_owner, m_isAngry));
			return;
		}
		else
		{
			//攻撃のクールタイムが0なら
			if (coll->GetAttackCoolTime() <= 0)
			{
				ThinkAttack(actorManager);
				return;
			}
		}
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
