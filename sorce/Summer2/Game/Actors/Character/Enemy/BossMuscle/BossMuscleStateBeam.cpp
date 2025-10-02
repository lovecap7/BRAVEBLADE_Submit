#include "BossMuscleStateBeam.h"
#include "BossMuscleStateIdle.h"
#include "BossMuscleStateAngry.h"
#include "BossMuscleStateDeath.h"
#include "BossMuscleStateHit.h"
#include "BossMuscle.h"
#include "../../../ActorManager.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Collision/SphereCollider.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../../General/Effect/MyEffect.h"
#include "../../../Attack/ULT.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//攻撃判定リセットの頻度
	constexpr int kBeamResetFrame = 7;
	//右手と左手のインデックス
	constexpr int kRightHandIndex = 57;
	constexpr int kLeftHandIndex = 29;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 150;
	//怒り状態の時の速度2倍
	constexpr float kAngryMoveSpeedRate = 2.0f;
	//モデルの向き調整
	const Vector2 kAdjustDir = Vector2(0.05f, 0.0f);
	//ビームの射程
	constexpr float kBeamRange = 5000.0f;
	//向きの更新フレーム調整
	constexpr int kUpdateDirFrame = 60;
}

BossMuscleStateBeam::BossMuscleStateBeam(std::weak_ptr<Actor> owner, bool isAngry, const std::weak_ptr<ActorManager> actorManager):
	BossMuscleStateBase(owner, isAngry),
	m_attackCountFrame(0)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kBeamName);
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//相手のほうを向く
	coll->LookAtTarget();
	//チャージエフェクト
	m_beamChargeEff = EffekseerManager::GetInstance().CreateEffect("BossBeamChargeEff", m_owner.lock()->GetPos());
	//VC
	coll->CharacterVC("Attack");
}

BossMuscleStateBeam::~BossMuscleStateBeam()
{
	int coolTime = kAttackCoolTime;
	//怒り状態ならクールタイムを短くする
	if (m_isAngry)coolTime /= 2;
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetAttackCoolTime(coolTime);
	//エフェクトの削除
	if (!m_beamChargeEff.expired())
	{
		m_beamChargeEff.lock()->Delete();
	}
	if( !m_beamEff.expired())
	{
		m_beamEff.lock()->Delete();
	}
	//攻撃の削除
	if (!m_beam.expired())
	{
		m_beam.lock()->Delete();
	}
	//グループに所属しているなら攻撃権を消す
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossMuscleStateBeam::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossMuscleStateBeam::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
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
	//カウント
	++m_attackCountFrame;
	//ビームのチャージエフェクトの位置更新
	auto model = coll->GetModel();
	Vector3 rightHand = MV1GetFramePosition(model->GetModelHandle(), kRightHandIndex);
	Vector3 leftHand = MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex);
	VECTOR pos = ((rightHand + leftHand) * 0.5f).ToDxLibVector(); //両手の中間位置
	if (!m_beamChargeEff.expired())
	{
		m_beamChargeEff.lock()->SetPos(pos);
	}
	//攻撃発生フレーム
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		CreateAttack(pos,actorManager);
	}
	//発生前まで向きを向きを変更
	else if (m_attackCountFrame < kUpdateDirFrame || m_isAngry)
	{
		//相手のほうを向く
		coll->LookAtTarget();
	}
	//怒り状態の時は追従を強く
	if (m_isAngry)
	{
		//位置更新
		UpdateBeamPos(pos, model, coll);
	}

	//攻撃判定をリセット
	if (m_attackCountFrame % kBeamResetFrame == 0)
	{
		if (!m_beam.expired())m_beam.lock()->ResetHitId();
	}
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		//待機状態に戻す
		ChangeState(std::make_shared<BossMuscleStateIdle>(m_owner,m_isAngry));
		return;
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossMuscleStateBeam::UpdateBeamPos(const Vector3 pos, std::shared_ptr<Model> model, std::shared_ptr<BossMuscle> coll)
{
	//エフェクトの位置
	Vector3 startPos = (pos + coll->GetPos()) * 0.5f;
	startPos.y = pos.y;
	//攻撃の向き
	auto dir = model->GetDir().XZ() + kAdjustDir;
	if (!m_beamEff.expired())
	{
		//必殺エフェクト
		auto eff = m_beamEff.lock();
		eff->SetPos(startPos);
		eff->LookAt(dir.XZ());
	}
	if (!m_beam.expired())
	{
		auto beem = m_beam.lock();
		//攻撃の位置
		Vector3 endPos = startPos + (dir.XZ() * kBeamRange);
		//座標をセット
		beem->SetStartPos(startPos);
		beem->SetEndPos(endPos);
	}
}

void BossMuscleStateBeam::CreateAttack(VECTOR pos, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//作成と参照
	auto attack = std::dynamic_pointer_cast<ULT>(actorManager.lock()->CreateAttack(AttackType::ULT, m_owner).lock());
	//攻撃を作成
	auto data = m_attackData;
	//大きさ
	attack->SetRadius(data.radius);
	//ダメージ、持続フレーム、ノックバックの大きさ、攻撃の重さ、ヒットストップの長さ、カメラの揺れ
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
	m_beam = attack; //攻撃の参照を保存
	//ビームエフェクト
	m_beamEff = EffekseerManager::GetInstance().CreateEffect("BossBeamEff", m_owner.lock()->GetPos());
	//攻撃の位置
	UpdateBeamPos(pos, coll->GetModel(), coll);
}
