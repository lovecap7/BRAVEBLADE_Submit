#include "BossDragonStateBreathAttack.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateHit.h"
#include "BossDragonStateIdle.h"
#include "BossDragon.h"
#include "../EnemyBase.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../ActorManager.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../Attack/Breath.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//弾の発射角度
	constexpr float kBulletAngle = 30.0f / 180.0f * DX_PI_F;
	//弾の生成位置のY座標
	constexpr float kBulletCreatePosY = 150.0f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 80;
}


BossDragonStateBreathAttack::BossDragonStateBreathAttack(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager) :
	BossDragonStateBase(owner),
	m_attackCountFrame(0)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kBreathName);
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//通常攻撃
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//相手のほうを向く
	coll->LookAtTarget();
	//VC
	coll->CharacterVC("Attack");
}

BossDragonStateBreathAttack::~BossDragonStateBreathAttack()
{
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
	//グループに所属しているなら攻撃権を消す
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossDragonStateBreathAttack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossDragonStateBreathAttack::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
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
		CreateAttack(actorManager);
	}
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		//待機状態に戻す
		ChangeState(std::make_shared<BossDragonStateIdle>(m_owner));
		return;
	}

	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossDragonStateBreathAttack::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//作成と参照
	m_attack1 = std::dynamic_pointer_cast<Breath>(actorManager.lock()->CreateAttack(AttackType::Breath, m_owner).lock());
	m_attack2 = std::dynamic_pointer_cast<Breath>(actorManager.lock()->CreateAttack(AttackType::Breath, m_owner).lock());
	m_attack3 = std::dynamic_pointer_cast<Breath>(actorManager.lock()->CreateAttack(AttackType::Breath, m_owner).lock());
	//弾の設定
	SetupBreath(m_attack1, 0.0f);
	SetupBreath(m_attack2, kBulletAngle);
	SetupBreath(m_attack3, -kBulletAngle);
	//エフェクト
	EffekseerManager::GetInstance().CreateTrackActorEffect("BreathEff", std::dynamic_pointer_cast<Actor>(m_attack1.lock()));
	EffekseerManager::GetInstance().CreateTrackActorEffect("BreathEff", std::dynamic_pointer_cast<Actor>(m_attack2.lock()));
	EffekseerManager::GetInstance().CreateTrackActorEffect("BreathEff", std::dynamic_pointer_cast<Actor>(m_attack3.lock()));
}


void BossDragonStateBreathAttack::SetupBreath(std::weak_ptr<Breath> bullet, float angle)
{
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//弾の設定
	auto attack = bullet.lock();
	auto data = m_attackData;
	//大きさ
	attack->SetRadius(data.radius);
	//ダメージ、持続フレーム、ノックバックの大きさ、攻撃の重さ、ヒットストップの長さ、カメラの揺れ
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
	//生成位置
	Vector3 bulletPos = coll->GetPos();
	bulletPos.y += kBulletCreatePosY;
	attack->SetPos(bulletPos);
	//弾の進行方向とスピード
	auto vec = coll->GetModel()->GetDir() * data.moveSpeed;
	vec = Quaternion::AngleAxis(angle, Vector3::Up()) * vec;
	attack->SetVec(vec);
}
