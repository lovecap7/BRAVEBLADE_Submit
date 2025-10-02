#include "BomberStateChase.h"
#include "BomberStateDeath.h"
#include "BomberStateHit.h"
#include "BomberStateIdle.h"
#include "BomberStateAttack.h"
#include "Bomber.h"
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
	constexpr float kAttackDistance = 400.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーション
	const char* kAnim = "CharacterArmature|Run";
	//プレイヤーを追いかける速度
	constexpr float kChaseSpeed = 3.0f;
}

BomberStateChase::BomberStateChase(std::weak_ptr<Actor> owner) :
	BomberStateBase(owner)
{
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	coll->SetCollState(CollisionState::Move);
	//アニメーション
	coll->GetModel()->SetAnim(kAnim, true);
}

BomberStateChase::~BomberStateChase()
{
}

void BomberStateChase::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BomberStateChase::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//コライダブル
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BomberStateDeath>(m_owner));
		return;
	}
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BomberStateHit>(m_owner));
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
			if (coll->GetAttackCoolTime() <= 0 && coll->CanAttack())
			{
				//攻撃状態にする
				ChangeState(std::make_shared<BomberStateAttack>(m_owner, actorManager));
				return;
			}
		}
		//射程範囲外なので
		else
		{
			//モデル
			auto model = coll->GetModel();
			//移動ベクトル
			Vector3 moveVec = GetNextNomVecFromRayCast(coll, targetData.targetPos, targetData.targetDirXZ, kChaseSpeed);
			//移動方向を見る
			model->SetDir(moveVec.XZ());
			//プレイヤーをに近づく
			coll->GetRb()->SetMoveVec(model->GetDir() * kChaseSpeed);
			return;
		}
	}
	//待機状態
	ChangeState(std::make_shared<BomberStateIdle>(m_owner));
	return;
}
