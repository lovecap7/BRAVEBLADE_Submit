#include "BossMuscleStateHit.h"
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

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーション
	const char* kAnim = "Boss|Hit";
}
BossMuscleStateHit::BossMuscleStateHit(std::weak_ptr<Actor> owner, bool isAngry) :
	BossMuscleStateBase(owner,isAngry)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//やられ
	coll->GetModel()->SetAnim(kAnim, false);
	//VC
	coll->CharacterVC("Damage");
}

BossMuscleStateHit::~BossMuscleStateHit()
{
}

void BossMuscleStateHit::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossMuscleStateHit::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
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
	auto model = coll->GetModel();
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		//初めから
		model->ReplayAnim();
	}
	//モデルのアニメーションが終わったら
	if (model->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<BossMuscleStateIdle>(m_owner, m_isAngry));
		return;
	}

	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
