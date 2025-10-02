#include "BossMuscleStateRightPunch.h"
#include "BossMuscleStateIdle.h"
#include "BossMuscleStateDeath.h"
#include "BossMuscleStateHit.h"
#include "BossMuscle.h"
#include "../EnemyBase.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../ActorManager.h"
#include "../../../Attack/Strike.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//右腕と右手のインデックス
	constexpr int kRightArmIndex = 41;
	constexpr int kRightHandIndex = 43;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 120;
	//怒り状態の時の速度2倍
	constexpr float kAngryMoveSpeedRate = 2.0f;
	//前進距離
	constexpr float kAttackMoveDistance = 200.0f;
}

BossMuscleStateRightPunch::BossMuscleStateRightPunch(std::weak_ptr<Actor> owner, bool isAngry,const std::weak_ptr<ActorManager> actorManager) :
	BossMuscleStateBase(owner, isAngry),
	m_attackCountFrame(0)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kRightPunchName);
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//相手のほうを向く
	coll->LookAtTarget();
	//VC
	coll->CharacterVC("Attack");
}

BossMuscleStateRightPunch::~BossMuscleStateRightPunch()
{
	auto coolTime = kAttackCoolTime;
	//怒り状態ならクールタイムを短くする
	if (m_isAngry)coolTime *= 0.5f;
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetAttackCoolTime(coolTime);
	if (!m_attack.expired())m_attack.lock()->Delete();
	//グループに所属しているなら攻撃権を消す
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossMuscleStateRightPunch::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossMuscleStateRightPunch::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
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
		ChangeState(std::make_shared<BossMuscleStateHit>(m_owner,m_isAngry));
		return;
	}
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		CreateAttack(actorManager);
	}
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		//待機状態に戻す
		ChangeState(std::make_shared<BossMuscleStateIdle>(m_owner, m_isAngry));
		return;
	}
	//攻撃の位置更新
	if (!m_attack.expired()) UpdateAttackPos();

	//移動フレーム中は前に進む
	if (m_attackCountFrame <= m_attackData.moveFrame)
	{
		//プレイヤーが遠い場合のみ
		auto targetData = coll->GetTargetData();
		if (targetData.targetDis >= kAttackMoveDistance)
		{
			//前進
			AttackMove();
		}
	}
	else
	{
		//減速
		coll->GetRb()->SpeedDown(kMoveDeceRate);
	}
}

void BossMuscleStateRightPunch::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//作成と参照
	m_attack = std::dynamic_pointer_cast<Strike>(actorManager.lock()->CreateAttack(AttackType::Strike, m_owner).lock());
	//攻撃を作成
	auto attack = m_attack.lock();
	auto data = m_attackData;
	//大きさ
	attack->SetRadius(data.radius);
	//ダメージ、持続フレーム、ノックバックの大きさ、攻撃の重さ、ヒットストップの長さ、カメラの揺れ
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
}

void BossMuscleStateRightPunch::UpdateAttackPos()
{
	auto model = m_owner.lock()->GetModel();
	//腕と手の座標
	VECTOR arm = MV1GetFramePosition(model->GetModelHandle(), kRightArmIndex);//右腕
	VECTOR hand = MV1GetFramePosition(model->GetModelHandle(), kRightHandIndex);//手の指先
	//座標をセット
	m_attack.lock()->SetStartPos(arm);
	m_attack.lock()->SetEndPos(hand);
}

void BossMuscleStateRightPunch::AttackMove()
{
	//向いてる方向に移動
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//怒り状態になっているなら速度2倍
	float moveSpeed = m_attackData.moveSpeed;
	if (m_isAngry) moveSpeed = moveSpeed * kAngryMoveSpeedRate;
	coll->GetRb()->SetMoveVec(coll->GetModel()->GetDir() * moveSpeed);
}