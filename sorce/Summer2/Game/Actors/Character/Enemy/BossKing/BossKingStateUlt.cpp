#include "BossKingStateUlt.h"
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
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../Attack/HomingBullet.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../../General/Effect/MyEffect.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 30;
	//弾の高さ
	constexpr float kBulletPosY = 150.0f;
	//追跡力
	constexpr float kTrackingRate = 0.08f;
	//発生間隔
	constexpr int kShotDelayFrame = 20;
	//魔法陣の位置調整
	constexpr float kMagicCirclePosDis = 500.0f;
}

BossKingStateUlt::BossKingStateUlt(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager):
	BossKingStateBase(owner, true),
	m_attackCountFrame(0),
	m_shotNum(1)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kUltName);
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//相手のほうを向く
	coll->LookAtTarget();
	//前後左右
	auto centerPos = coll->GetPos();
	Vector3 frontPos = Vector3::Forward() * kMagicCirclePosDis + centerPos;
	Vector3 backPos = Vector3::Back() * kMagicCirclePosDis + centerPos;
	Vector3 rightPos = Vector3::Right() * kMagicCirclePosDis + centerPos;
	Vector3 leftPos = Vector3::Left() * kMagicCirclePosDis + centerPos;
	//エフェクト
	auto& effectManager = EffekseerManager::GetInstance();
	effectManager.CreateEffect("BigMagicCircleEff", centerPos);
	effectManager.CreateEffect("MagicCircleEff", frontPos);
	effectManager.CreateEffect("MagicCircleEff", backPos);
	effectManager.CreateEffect("MagicCircleEff", rightPos);
	effectManager.CreateEffect("MagicCircleEff", leftPos);
	//VC
	coll->CharacterVC("Ult");
	//SE
	coll->CharacterOnceSE("MagicCircle");
}
BossKingStateUlt::~BossKingStateUlt()
{
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
	//グループに所属しているなら攻撃権を消す
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossKingStateUlt::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossKingStateUlt::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
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
	if (m_attackCountFrame == m_attackData.startFrame + kShotDelayFrame * m_shotNum)
	{
		//打った数をカウント
		++m_shotNum;
		m_shotNum = MathSub::ClampInt(m_shotNum, 0, m_attackData.attackNum);
		//前後左右
		auto centerPos = coll->GetPos();
		Vector3 frontPos = Vector3::Forward() * kMagicCirclePosDis + centerPos;
		Vector3 backPos = Vector3::Back() * kMagicCirclePosDis + centerPos;
		Vector3 rightPos = Vector3::Right() * kMagicCirclePosDis + centerPos;
		Vector3 leftPos = Vector3::Left() * kMagicCirclePosDis + centerPos;
		CreateAttack(actorManager, frontPos);
		CreateAttack(actorManager, backPos);
		CreateAttack(actorManager, rightPos);
		CreateAttack(actorManager, leftPos);
	}
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		//VC
		coll->CharacterVC("Change");
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


void BossKingStateUlt::CreateAttack(const std::weak_ptr<ActorManager> actorManager, Vector3 pos)
{
	//作成と参照
	auto attack = std::dynamic_pointer_cast<HomingBullet>(actorManager.lock()->CreateAttack(AttackType::Homing, m_owner).lock());
	//弾の設定
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	auto data = m_attackData;
	//大きさ
	attack->SetRadius(data.radius);
	//ダメージ、持続フレーム、ノックバックの大きさ、攻撃の重さ、ヒットストップの長さ、カメラの揺れ
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
	//生成位置
	Vector3 bulletPos = pos;
	//高さ
	bulletPos.y = kBulletPosY;
	attack->SetPos(bulletPos);
	//弾の進行方向とスピード
	auto vec = Vector3::Up() + Vector3::GetRandVecXZ();
	attack->SetVec(vec);
	//ターゲット
	attack->SetTarget(actorManager.lock()->GetPlayer());
	//追跡力
	attack->SetTrackingRate(kTrackingRate);
	//速度
	attack->SetSpeed(data.moveSpeed);
	//エフェクト
	EffekseerManager::GetInstance().CreateTrackActorEffect("BreathEff", attack);
}

