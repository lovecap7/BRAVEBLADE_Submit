#include "BossKingStateRandMove.h"
#include "BossKingStateIdle.h"
#include "BossKingStateHit.h"
#include "BossKingStateDeath.h"
#include "BossKingStateChase.h"
#include "BossKingStateChange.h"
#include "BossKing.h"
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
	const char* kAnim = "Boss3|Run";
	//速度
	constexpr float kMoveSpeed = 9.0f;
	//移動フレーム
	constexpr int kMoveFrame = 40;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 5;
}

BossKingStateRandMove::BossKingStateRandMove(std::weak_ptr<Actor> owner, bool isTransSecond) :
	BossKingStateBase(owner, isTransSecond),
	m_moveFrame(kMoveFrame)
{
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetCollState(CollisionState::Move);
	//アニメーション
	coll->GetModel()->SetAnim(kAnim, true);
	//ランダムな方向に移動
	m_moveVec = Vector3::GetRandVecXZ() * kMoveSpeed;
	coll->GetRb()->SetMoveVec(m_moveVec);
	//モデルの向き
	coll->GetModel()->SetDir(m_moveVec.XZ());
}
BossKingStateRandMove::~BossKingStateRandMove()
{
	auto coolTime = kAttackCoolTime;
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetAttackCoolTime(coolTime);
	//グループに所属しているなら攻撃権を消す
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossKingStateRandMove::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossKingStateRandMove::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//コライダブル
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	auto targetData = coll->GetTargetData();
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
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossKingStateHit>(m_owner, m_isTransformSecond));
		return;
	}
	--m_moveFrame;
	if (m_moveFrame < 0 || coll->IsWall())
	{
		//待機状態
		ChangeState(std::make_shared<BossKingStateIdle>(m_owner, m_isTransformSecond));
		return;
	}
	//移動
	coll->GetRb()->SetMoveVec(m_moveVec);
	//モデルの向き
	coll->GetModel()->SetDir(m_moveVec.XZ());
}
