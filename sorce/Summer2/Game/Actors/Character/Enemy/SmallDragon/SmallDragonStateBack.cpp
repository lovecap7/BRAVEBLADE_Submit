#include "SmallDragonStateBack.h"
#include "SmallDragonStateDeath.h"
#include "SmallDragonStateIdle.h"
#include "SmallDragonStateHit.h"
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
	//プレイヤー戦闘状態になる距離
	constexpr float kBackDistance = 300.0f;
	//アニメーション
	const char* kAnim = "CharacterArmature|Fast_Flying";
	//プレイヤーから離れる速度
	constexpr float kBackSpeed = 1.0f;
}

SmallDragonStateBack::SmallDragonStateBack(std::weak_ptr<Actor> owner) :
	SmallDragonStateBase(owner)
{
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Move);
}

SmallDragonStateBack::~SmallDragonStateBack()
{
}

void SmallDragonStateBack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void SmallDragonStateBack::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
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
		//下がる距離
		if (targetData.targetDis <= kBackDistance)
		{
			//下がる
			coll->GetRb()->SetMoveVec(targetData.targetDirXZ * -kBackSpeed);
		}
		else 
		{
			//待機
			ChangeState(std::make_shared<SmallDragonStateIdle>(m_owner));
			return;
		}
	}
	else
	{
		//待機
		ChangeState(std::make_shared<SmallDragonStateIdle>(m_owner));
		return;
	}
}