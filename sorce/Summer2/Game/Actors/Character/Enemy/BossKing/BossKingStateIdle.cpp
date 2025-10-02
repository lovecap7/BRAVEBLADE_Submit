#include "BossKingStateIdle.h"
#include "BossKingStateHit.h"
#include "BossKingStateDeath.h"
#include "BossKingStateChase.h"
#include "BossKingStateChange.h"
#include "BossKing.h"
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
	const char* kNormalAnim = "Boss3|Idle";//待機
}

BossKingStateIdle::BossKingStateIdle(std::weak_ptr<Actor> owner, bool isTransformSecond):
	BossKingStateBase(owner,isTransformSecond)
{
	//待機状態
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	//通常の待機
	coll->GetModel()->SetAnim(kNormalAnim, true);
	coll->SetCollState(CollisionState::Normal);
}


BossKingStateIdle::~BossKingStateIdle()
{
}

void BossKingStateIdle::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossKingStateIdle::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//コライダブル
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	auto targetData = coll->GetTargetData();
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
		ChangeState(std::make_shared<BossKingStateHit>(m_owner,m_isTransformSecond));
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
			ChangeState(std::make_shared<BossKingStateChase>(m_owner, m_isTransformSecond));
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
