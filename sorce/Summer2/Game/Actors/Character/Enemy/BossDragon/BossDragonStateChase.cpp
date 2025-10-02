#include "BossDragonStateChase.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateHit.h"
#include "BossDragonStatePunchAttack.h"
#include "BossDragonStateIdle.h"
#include "BossDragon.h"
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
	const char* kAnim = "CharacterArmature|Fast_Flying";
	//プレイヤーを追いかける速度
	constexpr float kChaseSpeed = 8.0f;
}


BossDragonStateChase::BossDragonStateChase(std::weak_ptr<Actor> owner) :
	BossDragonStateBase(owner)
{
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	coll->SetCollState(CollisionState::Move);
	//アニメーション
	coll->GetModel()->SetAnim(kAnim, true);
}

BossDragonStateChase::~BossDragonStateChase()
{
}

void BossDragonStateChase::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossDragonStateChase::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//コライダブル
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BossDragonStateDeath>(m_owner));
		return;
	}
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossDragonStateHit>(m_owner));
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
		//攻撃の距離
		if (targetData.targetDis <= kAttackDistance)
		{
			//攻撃のクールタイムが0なら
			if (coll->GetAttackCoolTime() <= 0)
			{
				//攻撃状態にする
				ChangeState(std::make_shared<BossDragonStatePunchAttack>(m_owner, actorManager));
				return;
			}
		}
		//射程範囲外なので
		else
		{
			//プレイヤーをに近づく
			coll->GetRb()->SetMoveVec(targetData.targetDirXZ * kChaseSpeed);
			return;
		}
	}
	//待機状態
	ChangeState(std::make_shared<BossDragonStateIdle>(m_owner));
	return;
}
