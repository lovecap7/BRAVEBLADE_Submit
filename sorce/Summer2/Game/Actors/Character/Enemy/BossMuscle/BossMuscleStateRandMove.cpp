#include "BossMuscleStateRandMove.h"
#include "BossMuscleStateIdle.h"
#include "BossMuscleStateDeath.h"
#include "BossMuscleStateHit.h"
#include "BossMuscle.h"
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
	//アニメーション
	const char* kAnim = "Boss|Run";
	//速度
	constexpr float kMoveSpeed = 9.0f;
	//移動フレーム
	constexpr int kMoveFrame = 120;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 30;
}

BossMuscleStateRandMove::BossMuscleStateRandMove(std::weak_ptr<Actor> owner, bool isAngry):
	BossMuscleStateBase(owner, isAngry),
	m_moveFrame(kMoveFrame)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetCollState(CollisionState::Move);
	//アニメーション
	coll->GetModel()->SetAnim(kAnim, true);
	//ランダムな方向に移動
	m_moveVec = Vector3::GetRandVecXZ() * kMoveSpeed;
	coll->GetRb()->SetMoveVec(m_moveVec);
	//モデルの向き
	coll->GetModel()->SetDir(m_moveVec.XZ());
}
BossMuscleStateRandMove::~BossMuscleStateRandMove()
{
	auto coolTime = kAttackCoolTime;
	//怒り状態ならクールタイムを短くする
	if (m_isAngry)coolTime *= 0.5f;
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetAttackCoolTime(coolTime);
	//グループに所属しているなら攻撃権を消す
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossMuscleStateRandMove::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossMuscleStateRandMove::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BossMuscleStateDeath>(m_owner));
		return;
	}
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossMuscleStateHit>(m_owner, m_isAngry));
		return;
	}
	--m_moveFrame;
	if(m_moveFrame < 0 || coll->IsWall())
	{
		//待機状態
		ChangeState(std::make_shared<BossMuscleStateIdle>(m_owner, m_isAngry));
		return;
	}
	//移動
	coll->GetRb()->SetMoveVec(m_moveVec);
	//モデルの向き
	coll->GetModel()->SetDir(m_moveVec.XZ());
}
