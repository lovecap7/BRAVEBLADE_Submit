#include "BossKingStateImpact.h"
#include "BossKingStateRapidFire.h"
#include "BossKingStateIdle.h"
#include "BossKingStateHit.h"
#include "BossKingStateDeath.h"
#include "BossKingStateChase.h"
#include "BossKingStateChange.h"
#include "BossKing.h"
#include "../EnemyBase.h"
#include "../../Player/Player.h"
#include "../../../ActorManager.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../General/Sound/SoundManager.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../Attack/AreaOfEffectAttack.h"
#include "../../../../../General/Effect/EffekseerManager.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 30;
	//攻撃位置調整
	constexpr float kAttackPosOffset = 200.0f;
}

BossKingStateImpact::BossKingStateImpact(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager):
	BossKingStateBase(owner, true),
	m_attackCountFrame(0)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kImpactName);
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//相手のほうを向く
	coll->LookAtTarget();
	//VC
	RandAttackVC();
}

BossKingStateImpact::~BossKingStateImpact()
{
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
	//グループに所属しているなら攻撃権を消す
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossKingStateImpact::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossKingStateImpact::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
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
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossKingStateHit>(m_owner, m_isTransformSecond));
		return;
	}
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		//SE
		SoundManager::GetInstance().PlayOnceSE("Blast");
		CreateAttack(actorManager);
	}
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		//待機状態
		ChangeState(std::make_shared<BossKingStateIdle>(m_owner, m_isTransformSecond));
		return;
	}

	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossKingStateImpact::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//作成と参照
	auto attack = std::dynamic_pointer_cast<AreaOfEffectAttack>(actorManager.lock()->CreateAttack(AttackType::AreaOfEffect, m_owner).lock());
	auto data = m_attackData;
	//大きさ
	attack->SetRadius(data.radius);
	//位置
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	auto pos = coll->GetPos();
	pos += coll->GetModel()->GetDir() * kAttackPosOffset;
	attack->SetPos(pos);
	//ダメージ、持続フレーム、ノックバックの大きさ、攻撃の重さ、ヒットストップの長さ、カメラの揺れ
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
	//爆発
	EffekseerManager::GetInstance().CreateEffect("ExplosionEff", pos);
}