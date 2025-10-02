#include "BossDragonStateSweepAttack.h"
#include "BossDragonStateHit.h"
#include "BossDragonStateIdle.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateBase.h"
#include "BossDragon.h"
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
	//攻撃の全体フレーム
	constexpr int kAllAttackFrame = 110;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//左腕と左手のインデックス
	constexpr int kLeftArmIndex = 31;
	constexpr int kLeftHandIndex = 36;
	//予備動作中の回転
	constexpr float kStartRotaAngle = 1.0f;
	//アニメーション
	const char* kStartAnim = "CharacterArmature|No";
	//アニメーションの速度
	constexpr float kStartAnimSpeed = 0.5f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 30;
}

BossDragonStateSweepAttack::BossDragonStateSweepAttack(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager) :
	BossDragonStateBase(owner),
	m_attackCountFrame(0)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kSweepName);
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//通常攻撃
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(kStartAnim, false, kStartAnimSpeed);
	//相手のほうを向く
	coll->LookAtTarget();
	//回転量
	m_attackRotaAngle = -(360.0f + kStartRotaAngle * m_attackData.startFrame) / static_cast<float>(m_attackData.keepFrame);
	//VC
	coll->CharacterVC("Attack");
}

BossDragonStateSweepAttack::~BossDragonStateSweepAttack()
{
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
	if (!m_attack.expired())m_attack.lock()->Delete();
	//グループに所属しているなら攻撃権を消す
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossDragonStateSweepAttack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossDragonStateSweepAttack::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BossDragonStateDeath>(m_owner));
		return;
	}
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossDragonStateHit>(m_owner));
		return;
	}
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
		CreateAttack(actorManager);
	}
	//攻撃が終了するまで回転
	auto model = coll->GetModel();
	if (m_attackCountFrame >= m_attackData.startFrame)
	{
		//攻撃判定中回転
		if (!m_attack.expired())
		{
			model->SetRot(VGet(0.0f, m_attackRotaAngle, 0.0f));
		}
	}
	else
	{
		//最初の予備動作中も少し回転
		model->SetRot(VGet(0.0f, kStartRotaAngle, 0.0f));
	}

	//攻撃終了
	if (m_attackCountFrame >= kAllAttackFrame)
	{
		//待機状態に戻す
		ChangeState(std::make_shared<BossDragonStateIdle>(m_owner));
		return;
	}
	//攻撃の位置更新
	if (!m_attack.expired()) UpdateAttackPos();

	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossDragonStateSweepAttack::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
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

void BossDragonStateSweepAttack::UpdateAttackPos()
{
	auto model = m_owner.lock()->GetModel();
	//腕と手の座標
	VECTOR leftArm = MV1GetFramePosition(model->GetModelHandle(), kLeftArmIndex);//左腕
	VECTOR leftHand = MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex);//手の指先
	//座標をセット
	m_attack.lock()->SetStartPos(leftArm);
	m_attack.lock()->SetEndPos(leftHand);
}