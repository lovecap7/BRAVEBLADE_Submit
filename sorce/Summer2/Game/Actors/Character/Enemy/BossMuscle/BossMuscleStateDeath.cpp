#include "BossMuscleStateDeath.h"
#include "BossMuscle.h"
#include "../EnemyBase.h"
#include "../../../Actor.h"
#include "../../../ActorManager.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../General/Sound/SoundManager.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーション
	const char* kAnim = "Boss|Dead";
	//アニメーション速度
	constexpr float kAnimSpeed = 0.6f;
}

BossMuscleStateDeath::BossMuscleStateDeath(std::weak_ptr<Actor> owner) :
	BossMuscleStateBase(owner,true)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//死亡状態にする
	coll->SetCollState(CollisionState::Dead);
	//死亡
	coll->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//無敵
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
	//BGMを止める
	SoundManager::GetInstance().StopBGM();
	//VC
	coll->CharacterVC("Dead");
}

BossMuscleStateDeath::~BossMuscleStateDeath()
{
}

void BossMuscleStateDeath::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossMuscleStateDeath::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		coll->Delete();//削除
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
