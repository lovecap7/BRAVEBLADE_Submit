#include "BomberStateHit.h"
#include "BomberStateIdle.h"
#include "BomberStateDeath.h"
#include "Bomber.h"
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
	const char* kAnim = "CharacterArmature|HitReact";//小喰らい
}

BomberStateHit::BomberStateHit(std::weak_ptr<Actor> owner) :
	BomberStateBase(owner)
{
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	//やられ
	coll->GetModel()->SetAnim(kAnim, false);
	//VC
	coll->CharacterVC("Damage");
	//警戒
	coll->SetIsWarning(true);
}

BomberStateHit::~BomberStateHit()
{
}

void BomberStateHit::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BomberStateHit::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BomberStateDeath>(m_owner));
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
		ChangeState(std::make_shared<BomberStateIdle>(m_owner));
		return;
	}

	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
