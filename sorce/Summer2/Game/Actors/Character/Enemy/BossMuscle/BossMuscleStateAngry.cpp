#include "BossMuscleStateAngry.h"
#include "BossMuscleStateDeath.h"
#include "BossMuscleStateIdle.h"
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
#include "../../../../../General/Effect/EffekseerManager.h"
namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーションの名前
	const char* kAnim = "Boss|Change";
}

BossMuscleStateAngry::BossMuscleStateAngry(std::weak_ptr<Actor> owner) :
	BossMuscleStateBase(owner,true)
{
	//待機状態
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Normal);
	//VC
	coll->CharacterVC("Angry");
}

BossMuscleStateAngry::~BossMuscleStateAngry()
{
}

void BossMuscleStateAngry::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
	//怒りエフェクト
	EffekseerManager::GetInstance().CreateTrackActorEffect("BossAngryEff", m_owner);
}

void BossMuscleStateAngry::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//コライダブル
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BossMuscleStateDeath>(m_owner));
		return;
	}
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		//怒り待機状態に
		ChangeState(std::make_shared<BossMuscleStateIdle>(m_owner,m_isAngry));
		return;
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
