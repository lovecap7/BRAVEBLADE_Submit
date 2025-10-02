#include "SmallDragonStateIdle.h"
#include "SmallDragonStateAttack.h"
#include "SmallDragonStateHit.h"
#include "SmallDragonStateDeath.h"
#include "SmallDragonStateBack.h"
#include "SmallDragonStateSearch.h"
#include "SmallDragon.h"
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
	//後退する距離
	constexpr float kBackDistance = 300.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーションの名前
	const char* kAnim = "CharacterArmature|Flying_Idle";//待機
	//警戒フレーム(ターゲットを見失っても一定フレーム経過するまでは警戒)
	constexpr int kWarningFrame = 120;
}

SmallDragonStateIdle::SmallDragonStateIdle(std::weak_ptr<Actor> owner):
	SmallDragonStateBase(owner),
	m_warningFrame(kWarningFrame)
{
	//待機状態
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Normal);
}

SmallDragonStateIdle::~SmallDragonStateIdle()
{
}

void SmallDragonStateIdle::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void SmallDragonStateIdle::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//コライダブル
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<SmallDragonStateDeath>(m_owner));
		return;
	}
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<SmallDragonStateHit>(m_owner));
		return;
	}

	auto targetData = coll->GetTargetData();
	//プレイヤーを見つけたなら
	if (targetData.isHitTarget)
	{
		//プレイヤーを見る
		coll->LookAtTarget();
		//警戒
		m_warningFrame = kWarningFrame;
		coll->SetIsWarning(true);
		//攻撃のクールタイムが0なら
		if (coll->GetAttackCoolTime() <= 0 && coll->CanAttack())
		{
			//攻撃
			ChangeState(std::make_shared<SmallDragonStateAttack>(m_owner,actorManager));
			return;
		}
		//下がる距離
		if (targetData.targetDis <= kBackDistance)
		{
			//下がる
			ChangeState(std::make_shared<SmallDragonStateBack>(m_owner));
			return;
		}
	}
	//探索場所があるなら
	else if (coll->IsHaveSearchPlace())
	{
		--m_warningFrame;
		if (m_warningFrame <= 0)
		{
			//索敵状態にする
			ChangeState(std::make_shared<SmallDragonStateSearch>(m_owner));
			return;
		}
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}