#include "SmallDragonStateDeath.h"
#include "SmallDragon.h"
#include "../EnemyBase.h"
#include "../../../Actor.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../Camera/GameCamera/GameCamera.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーション
	const char* kAnim = "CharacterArmature|Death";
}

SmallDragonStateDeath::SmallDragonStateDeath(std::weak_ptr<Actor> owner) :
	SmallDragonStateBase(owner)
{
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	//死亡状態にする
	coll->SetCollState(CollisionState::Dead);
	//死亡
	coll->GetModel()->SetAnim(kAnim, false);
	//無敵
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
	//VC
	coll->CharacterVC("Dead");
}

SmallDragonStateDeath::~SmallDragonStateDeath()
{
}

void SmallDragonStateDeath::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void SmallDragonStateDeath::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		coll->Delete();//削除
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

