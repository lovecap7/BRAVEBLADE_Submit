#include "PurpleDinosaurStateIdle.h"
#include "PurpleDinosaurStateDeath.h"
#include "PurpleDinosaurStateChase.h"
#include "PurpleDinosaurStateAttack.h"
#include "PurpleDinosaurStateHit.h"
#include "PurpleDinosaurStateSearch.h"
#include "PurpleDinosaur.h"
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
	//プレイヤーに攻撃する距離
	constexpr float kAttackDistance = 200.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーションの名前
	const char* kAnim = "CharacterArmature|Idle";//待機
	//警戒フレーム(ターゲットを見失っても一定フレーム経過するまでは警戒)
	constexpr int kWarningFrame = 120;
}

PurpleDinosaurStateIdle::PurpleDinosaurStateIdle(std::weak_ptr<Actor> owner):
	PurpleDinosaurStateBase(owner),
	m_warningFrame(kWarningFrame)
{
	//待機状態
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Normal);
}

PurpleDinosaurStateIdle::~PurpleDinosaurStateIdle()
{
}

void PurpleDinosaurStateIdle::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateIdle::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//コライダブル
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<PurpleDinosaurStateDeath>(m_owner));
		return;
	}
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<PurpleDinosaurStateHit>(m_owner));
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

		//攻撃の距離
		if (targetData.targetDis <= kAttackDistance)
		{
			//攻撃のクールタイムが0なら
			if (coll->GetAttackCoolTime() <= 0 && coll->CanAttack())
			{
				//攻撃状態にする
				ChangeState(std::make_shared<PurpleDinosaurStateAttack>(m_owner,actorManager));
				return;
			}
		}
		//攻撃の距離じゃないときは近づく
		else
		{
			//プレイヤーをに近づく
			ChangeState(std::make_shared<PurpleDinosaurStateChase>(m_owner));
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
			ChangeState(std::make_shared<PurpleDinosaurStateSearch>(m_owner));
			return;
		}
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}