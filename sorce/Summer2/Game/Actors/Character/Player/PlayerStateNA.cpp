#include "PlayerStateNA.h"
#include "PlayerStateIdle.h"
#include "PlayerStateRolling.h"
#include "PlayerStateCharge.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "UltGage.h"
#include "../Enemy/EnemyBase.h"
#include "../../ActorManager.h"
#include "../../Attack/Slash.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../General/Effect/MyEffect.h"
#include "../../../../Game/Camera/GameCamera/GameCamera.h"

namespace
{
	//武器の座標と当たり判定の情報
	//右手の薬指のインデックス
	constexpr int kRightRingFingerIndex = 55;
	constexpr int kRightIndexFingerIndex = 43;
	//武器の長さ
	constexpr float kSwordHeight = 150.0f;
	//キャンセル
	constexpr int kAttackCancelFrame = 15;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//連続攻撃の最大数
	constexpr int kMaxComboNum = 4;
	//攻撃が強くなる段階数
	constexpr int kHighAttackComboNum = 3;
	//前進をする距離
	constexpr float kAttackMoveDistance = 100.0f;
	//全身を緩和する割合
	constexpr float kNearDistanceAttackMoveRate = 0.1f;
}

PlayerStateNA::PlayerStateNA(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager) :
	PlayerStateBase(player),
	m_comboNum(1),
	m_attackCountFrame(0)
{
	//VC
	RandAttackVC();
	//攻撃データの初期化
	InitAttackData(actorManager);
	//衝撃エフェクト
	EffekseerManager::GetInstance().CreateEffect("LowShockWaveEff", m_owner.lock()->GetPos());
}

PlayerStateNA::~PlayerStateNA()
{
	if (!m_eff.expired())
	{
		m_eff.lock()->Delete();
	}
}
void PlayerStateNA::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateNA::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//ボスを倒す
	if (actorManager.lock()->IsBossDisappear())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//ボスの体力がなくなった場合またはモデルのアニメーションが終わったら
	if (actorManager.lock()->IsBossDead() ||
		coll->GetModel()->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//死亡したかつボスが倒せてない場合
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_owner));
		return;
	}
	//攻撃を受けたなら
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_owner));
		return;
	}
	auto& input = Input::GetInstance();
	//カウント
	++m_attackCountFrame;
	auto model = coll->GetModel();
	//攻撃発生フレーム
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		//SE
		coll->CharacterOnceSE("NA");
		//攻撃作成
		CreateAttack(m_attackData.radius, m_attackData.damege, m_attackData.keepFrame, 
			m_attackData.knockBackPower, m_attackData.attackWeight, actorManager);
		if (!m_eff.expired())
		{
			//エフェクト削除
			m_eff.lock()->Delete();
		}
		//斬撃エフェクトを段階に応じて作成
		if (m_comboNum >= kHighAttackComboNum)
		{
			//斬撃2エフェクト
			m_eff = EffekseerManager::GetInstance().CreateEffect("SlashtTraject2Eff", m_owner.lock()->GetPos());
		}
		else
		{
			//斬撃1エフェクト
			m_eff = EffekseerManager::GetInstance().CreateEffect("SlashtTraject1Eff", m_owner.lock()->GetPos());
		}
	}
	//アニメーションのラスト数フレーム以内で入力があるなら2段回目の攻撃
	if (model->GetTotalAnimFrame() - kAttackCancelFrame <= model->GetNowAnimFrame())
	{
		//回避ボタンを押したら
		if (input.IsTrigger("A"))
		{
			//回避
			ChangeState(std::make_shared<PlayerStateRolling>(m_owner));
			return;
		}
		//まだ攻撃できるなら
		if (input.IsTrigger("X") && m_comboNum < kMaxComboNum)
		{
			//次の攻撃
			++m_comboNum;
			//初期化
			InitAttackData(actorManager);
			//衝撃エフェクト
			EffekseerManager::GetInstance().CreateEffect("LowShockWaveEff", m_owner.lock()->GetPos());
			m_attackCountFrame = 0;
			return;
		}
		//チャージボタンを押したら
		if (input.IsTrigger("Y"))
		{
			//チャージ
			ChangeState(std::make_shared<PlayerStateCharge>(m_owner, actorManager));
			return;
		}
	}
	//攻撃の位置更新
	UpdateAttackPos();
	//移動フレーム中は前に進む
	if (m_attackCountFrame <= m_attackData.moveFrame)
	{
		//速度
		float speed = m_attackData.moveSpeed;
		//ターゲット
		CharacterBase::TargetData targetData = coll->GetTargetData();
		//ターゲットが近いときは全身速度を落とす
		if (targetData.targetDis <= kAttackMoveDistance && targetData.isHitTarget)
		{
			speed *= kNearDistanceAttackMoveRate;
		}
		//前進
		AttackMove(speed);
	}
	else
	{
		//減速
		coll->GetRb()->SpeedDown(kMoveDeceRate);
	}
}
void PlayerStateNA::CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager)
{
	auto owner = m_owner.lock();
	//作成と参照
	m_attack = std::dynamic_pointer_cast<Slash>(actorManager.lock()->CreateAttack(AttackType::Slash, m_owner).lock());
	//攻撃を作成
	auto attack = m_attack.lock();
	auto data = m_attackData;
	//大きさ
	attack->SetRadius(data.radius);
	//ダメージ、持続フレーム、ノックバックの大きさ、攻撃の重さ、ヒットストップの長さ、カメラの揺れ
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
}
void PlayerStateNA::UpdateAttackPos()
{
	auto model = m_owner.lock()->GetModel();
	//武器
	//右手の薬指と人差し指の座標から武器の座標を出す
	VECTOR ringFinger = MV1GetFramePosition(model->GetModelHandle(), kRightRingFingerIndex);//薬指
	VECTOR indexFinger = MV1GetFramePosition(model->GetModelHandle(), kRightIndexFingerIndex);//人差し指
	//武器の矛先
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	VECTOR swordTip = VAdd(ringFinger, VScale(swordDir, kSwordHeight));//持ち手の座標に加算して剣先の座標を出す
	if (!m_attack.expired())
	{
		//座標をセット
		m_attack.lock()->SetStartPos(ringFinger);
		m_attack.lock()->SetEndPos(swordTip);
		if (!m_eff.expired())
		{
			auto eff = m_eff.lock();
			//エフェクトの位置更新
			eff->SetPos(((Vector3(ringFinger) + swordTip) / 2.0f));
		}
	}
}
void PlayerStateNA::AttackMove(float speed)
{
	//ターゲットを索敵してるなら(ターゲットのほうを向く)
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto targetData = coll->GetTargetData();
	//向き
	Vector3 dir = coll->GetPlayerStickVec().XZ();
	//ターゲットが発見できた時
	if (targetData.isHitTarget)
	{
		dir = targetData.targetDirXZ;
	}
	//向きの更新
	coll->GetModel()->SetDir(dir.XZ());
	//向いてる方向に移動
	coll->GetRb()->SetMoveVec(coll->GetModel()->GetDir() * speed);
}

void PlayerStateNA::InitAttackData(const std::weak_ptr<ActorManager> actorManager)
{
	std::string attackName;
	//攻撃の段階に合わせて攻撃の初期化
	switch (m_comboNum)
	{
	case 1:
		attackName = kNA1Name;
		break;
	case 2:
		attackName = kNA2Name;
		break;
	case 3:
		attackName = kNA3Name;
		break;
	case kMaxComboNum:
		attackName = kNA4Name;
		//VC
		RandAttackVC();
		break;
	default:
		break;
	}
	m_attackData = actorManager.lock()->GetAttackData(kPlayerName, attackName);
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//通常攻撃
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//加算ゲージの予約
	coll->GetUltGage().lock()->SetPendingUltGage(m_attackData.addUltGage);
}