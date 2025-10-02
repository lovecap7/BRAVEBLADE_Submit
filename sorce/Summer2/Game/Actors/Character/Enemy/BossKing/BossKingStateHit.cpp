#include "BossKingStateHit.h"
#include "BossKingStateDeath.h"
#include "BossKingStateIdle.h"
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
	//アニメーション
	const char* kAnim = "Boss3|Hit";
}

BossKingStateHit::BossKingStateHit(std::weak_ptr<Actor> owner, bool isTransformSecond):
	BossKingStateBase(owner,isTransformSecond)
{
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	//やられ
	coll->GetModel()->SetAnim(kAnim, false);
	//VC
	coll->CharacterVC("Damage");
}

BossKingStateHit::~BossKingStateHit()
{
}

void BossKingStateHit::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossKingStateHit::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
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
		ChangeState(std::make_shared<BossKingStateIdle>(m_owner,m_isTransformSecond));
		return;
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}