#include "BossKingStateBlastAttack.h"
#include "BossKingStateIdle.h"
#include "BossKingStateHit.h"
#include "BossKingStateDeath.h"
#include "BossKingStateChase.h"
#include "BossKingStateChange.h"
#include "BossKing.h"
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
#include "../../../Character/Player/Player.h"
#include "../../../Attack/Blast.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../../General/Effect/MyEffect.h"


namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 80;
	//追跡終了フレーム
	constexpr int kTrackFrameOffset = 10;
}

BossKingStateBlastAttack::BossKingStateBlastAttack(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager) :
	BossKingStateBase(owner, false),
	m_attackCountFrame(0)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kBlastAttackName);
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//相手のほうを向く
	coll->LookAtTarget();
	//必殺エフェクト
	auto player = actorManager.lock()->GetPlayer();
	if (!player.expired())
	{
		m_blastMagicEff = EffekseerManager::GetInstance().CreateEffect("BlastMagicEff", player.lock()->GetPos());
	}
	//VC
	RandAttackVC();
}

BossKingStateBlastAttack::~BossKingStateBlastAttack()
{
	auto coolTime = kAttackCoolTime;
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetAttackCoolTime(coolTime);
	//エフェクトを消す
	if (!m_blastMagicEff.expired())
	{
		m_blastMagicEff.lock()->Delete();
	}
	//グループに所属しているなら攻撃権を消す
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossKingStateBlastAttack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossKingStateBlastAttack::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//コライダブル
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
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		//待機状態に戻す
		ChangeState(std::make_shared<BossKingStateIdle>(m_owner, false));
		return;
	}
	//カウント
	++m_attackCountFrame;
	//魔法陣の位置更新
	auto player = actorManager.lock()->GetPlayer();
	if (!player.expired())
	{
		//発生前まで追跡
		if (m_attackCountFrame <= m_attackData.startFrame - kTrackFrameOffset)
		{
			UpdateBlastMagicEffPos(player.lock()->GetPos());
		}
	}
	//攻撃発生フレーム
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		if (!m_blastMagicEff.expired())
		{
			//エフェクトの位置に攻撃を出す
			auto model = coll->GetModel();
			CreateAttack(actorManager, m_blastMagicEff.lock()->GetPos());
			//エフェクトを消す
			m_blastMagicEff.lock()->Delete();
		}
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossKingStateBlastAttack::CreateAttack(const std::weak_ptr<ActorManager> actorManager, Vector3 pos)
{
	//作成と参照
	auto attack = std::dynamic_pointer_cast<Blast>(actorManager.lock()->CreateAttack(AttackType::Blast, m_owner).lock());
	//攻撃を作成
	auto data = m_attackData;
	//大きさ
	attack->SetRadius(data.radius);
	//中点
	attack->SetPos(pos);
	//ダメージ、持続フレーム、ノックバックの大きさ、攻撃の重さ、ヒットストップの長さ、カメラの揺れ
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
}

void BossKingStateBlastAttack::UpdateBlastMagicEffPos(Vector3 pos)
{
	if (!m_blastMagicEff.expired())
	{
		m_blastMagicEff.lock()->SetPos(pos);
	}
}
