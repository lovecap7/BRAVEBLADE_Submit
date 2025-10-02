#include "BossMuscleStateChase.h"
#include "BossMuscleStateIdle.h"
#include "BossMuscleStateDeath.h"
#include "BossMuscleStateHit.h"
#include "BossMuscle.h"
#include "../EnemyBase.h"
#include "../../../ActorManager.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
namespace
{
	//アニメーション
	const char* kAnim = "Boss|Walk";
	//プレイヤーを追いかける速度
	constexpr float kChaseSpeed = 2.0f;
}

BossMuscleStateChase::BossMuscleStateChase(std::weak_ptr<Actor> owner, bool isAngry):
	BossMuscleStateBase(owner, isAngry)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetCollState(CollisionState::Move);
	//アニメーション
	coll->GetModel()->SetAnim(kAnim, true);
}

BossMuscleStateChase::~BossMuscleStateChase()
{
}

void BossMuscleStateChase::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossMuscleStateChase::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//コライダブル
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
		ChangeState(std::make_shared<BossMuscleStateHit>(m_owner,m_isAngry));
		return;
	}
	//プレイヤー
	auto player = actorManager.lock()->GetPlayer();

	auto targetData = coll->GetTargetData();
	//プレイヤーを見つけたなら
	if (targetData.isHitTarget)
	{
		//プレイヤーを見る
		coll->LookAtTarget();
		//攻撃のクールタイムが0なら
		if (coll->GetAttackCoolTime() <= 0)
		{
			ThinkAttack(actorManager);
			return;
		}
		//近づく距離
		if (targetData.targetDis > kMeleeAttackDistance)
		{
			//プレイヤーをに近づく
			coll->GetRb()->SetMoveVec(targetData.targetDirXZ * kChaseSpeed);
			return;
		}
	}
	//ここまで来たら
	//待機状態
	ChangeState(std::make_shared<BossMuscleStateIdle>(m_owner, m_isAngry));
	return;
}