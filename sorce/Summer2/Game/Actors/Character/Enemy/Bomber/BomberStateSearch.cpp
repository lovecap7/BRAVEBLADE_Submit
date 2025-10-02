#include "BomberStateSearch.h"
#include "BomberStateDeath.h"
#include "BomberStateIdle.h"
#include "BomberStateHit.h"
#include "Bomber.h"
#include "../EnemyBase.h"
#include "../../../ActorManager.h"
#include "../../../SearchPlace.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../General/Collision/Physics.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
namespace
{
	//アニメーション
	const char* kAnim = "CharacterArmature|Walk";
	//速度
	constexpr float kMoveSpeed = 1.0f;
	//移動フレーム
	constexpr int kMoveFrame = 300;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 5;
	//レイキャストの許容値(自分とターゲット)
	constexpr int kRayCastToleranceNum = 2;
	//移動前レイキャストの許容値(自分)
	constexpr int kBeforeMoveToleranceNum = 1;
	//移動後のレイキャストの許容値
	constexpr int kAfterMoveToleranceNum = 0;
}

BomberStateSearch::BomberStateSearch(std::weak_ptr<Actor> owner) :
	BomberStateBase(owner),
	m_moveFrame(kMoveFrame)
{
	if (m_owner.expired())return;
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	//アニメーション
	coll->GetModel()->SetAnim(kAnim, true);
	//状態
	coll->SetCollState(CollisionState::Move);
	//ランダムな方向に移動
	m_moveVec = Vector3::GetRandVecXZ() * kMoveSpeed;
	coll->GetRb()->SetMoveVec(m_moveVec);
	//モデルの向き
	coll->GetModel()->SetDir(m_moveVec.XZ());
	//警戒はしていない
	coll->SetIsWarning(false);
}

BomberStateSearch::~BomberStateSearch()
{
}

void BomberStateSearch::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BomberStateSearch::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
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
	//壁に当たったら
	if (coll->IsWall() || m_moveFrame < 0.0f)
	{
		m_moveFrame = kMoveFrame;
		//ランダムな方向に移動
		m_moveVec = Vector3::GetRandVecXZ() * kMoveSpeed;
	}
	//ターゲット
	auto targetData = coll->GetTargetData();
	//モデル
	auto model = coll->GetModel();
	//索敵範囲外に出た時
	if (!coll->GetSearchPlace().expired())
	{
		std::shared_ptr<SearchPlace> searchPlace = coll->GetSearchPlace().lock();
		//範囲外
		if (!searchPlace->IsInSearchPlace(coll->GetPos()))
		{
			//目標地点
			Vector3 searchPlacePos = searchPlace->GetPos();
			//範囲内に向かって動く
			m_moveVec = searchPlacePos - coll->GetPos();
			//移動ベクトル(間に障害物がある場合避ける)
			m_moveVec = GetNextNomVecFromRayCast(coll, searchPlacePos, m_moveVec, kMoveSpeed, kBeforeMoveToleranceNum, kAfterMoveToleranceNum);
		}
	}
	//移動フレーム
	--m_moveFrame;
	//プレイヤーを見つけた
	if (targetData.isHitTarget)
	{
		//遮る物がないなら
		if (!Physics::GetInstance().RayCast(coll->GetPos(), targetData.targetPos).size() <= kRayCastToleranceNum)
		{
			//待機状態
			ChangeState(std::make_shared<BomberStateIdle>(m_owner));
			return;
		}
	}
	//モデルの向き
	model->SetDir(m_moveVec.XZ());
	//移動
	coll->GetRb()->SetMoveVec(model->GetDir() * kMoveSpeed);
}
