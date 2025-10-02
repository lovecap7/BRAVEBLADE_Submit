#include "BossKingStateChase.h"
#include "BossKingStateIdle.h"
#include "BossKingStateHit.h"
#include "BossKingStateDeath.h"
#include "BossKingStateChange.h"
#include "BossKing.h"
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
	//プレイヤー戦闘状態になる距離
	constexpr float kAttackDistance = 200.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーション
	const char* kAnim = "Boss3|Walk";
	//プレイヤーを追いかける速度
	constexpr float kChaseSpeed = 8.0f;
}


BossKingStateChase::BossKingStateChase(std::weak_ptr<Actor> owner, bool isTransformSecond):
	BossKingStateBase(owner,isTransformSecond)
{
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetCollState(CollisionState::Move);
	//アニメーション
	coll->GetModel()->SetAnim(kAnim, true);
}

BossKingStateChase::~BossKingStateChase()
{
}

void BossKingStateChase::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossKingStateChase::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
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
	ChangeState(std::make_shared<BossKingStateIdle>(m_owner,m_isTransformSecond));
	return;
}
