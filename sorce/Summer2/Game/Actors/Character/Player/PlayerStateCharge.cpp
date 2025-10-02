#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
#include "PlayerStateCA.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "PlayerStateWin.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "UltGage.h"
#include "../Enemy/EnemyBase.h"
#include "../../ActorManager.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../General/Effect/TrackActorEffect.h"
#include "../../../../General/Sound/SE.h"
#include "../../../../Game/Camera/GameCamera/GameCamera.h"

namespace
{
	//アニメーション
	const char* kAnim = "Player|Charge";//チャージ
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//硬直
	constexpr int kStopFrame = 30;
}

PlayerStateCharge::PlayerStateCharge(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager) :
	PlayerStateBase(player),
	m_chargeFrame(0)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//チャージ
	coll->GetModel()->SetAnim(kAnim, true);
	//チャージエフェクト
	m_chargeEff = EffekseerManager::GetInstance().CreateEffect(std::string("ChargeEff"), coll->GetPos());
	m_levelEff = EffekseerManager::GetInstance().CreateTrackActorEffect(std::string("ChargeLevel1Eff"), m_owner);
	//どのレベルまで溜めているかをみてエフェクトを変える準備
	//一つ下のレベルの持続フレームを超えたらレベルが上がる
	auto level2 = actorManager.lock()->GetAttackData(kPlayerName, kCA1Name);
	auto level3 = actorManager.lock()->GetAttackData(kPlayerName, kCA2Name);
	m_chargeLevel2Frame = level2.keepFrame;
	m_chargeLevel3Frame = level3.keepFrame;
	//1段階目
	m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kCA1Name);
	//SE
	coll->CharacterOnceSE("CARankUp");
	m_chargeSE = coll->CharacterLoopSE("CACharge");
}

PlayerStateCharge::~PlayerStateCharge()
{
	m_chargeEff.lock()->Delete();
	//エフェクトを数フレーム後削除
	m_levelEff.lock()->Delete();
	//SE削除
	if (!m_chargeSE.expired())
	{
		m_chargeSE.lock()->Delete();
	}
}
void PlayerStateCharge::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateCharge::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto& input = Input::GetInstance();
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//ボスを倒す
	if (actorManager.lock()->IsBossDisappear())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//ボスの体力がなくなった場合
	if (actorManager.lock()->GetBoss().lock()->GetHitPoints().lock()->IsDead())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//死亡した
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
	//回避ボタンを押したら
	if (input.IsTrigger("RB"))
	{
		//回避
		ChangeState(std::make_shared<PlayerStateRolling>(m_owner));
		return;
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
	//向きの更新
	Vector2 dir = coll->GetPlayerStickVec();
	coll->GetModel()->SetDir(dir);
	//溜めてる時
	if (input.IsPress("Y") || m_chargeFrame < kStopFrame)
	{
		//タメ攻撃チャージ
		++m_chargeFrame;
		//チャージエフェクト
		m_chargeEff.lock()->SetPos(coll->GetPos());
		//2段階目
		if (m_chargeFrame == m_chargeLevel2Frame)
		{
			//SE
			coll->CharacterOnceSE("CARankUp");
			m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kCA2Name);
			//段階ごとにエフェクトを変更
			m_levelEff.lock()->Delete();
			m_levelEff = EffekseerManager::GetInstance().CreateTrackActorEffect(std::string("ChargeLevel2Eff"), m_owner);
		}
		//3段階目
		else if(m_chargeFrame == m_chargeLevel3Frame)
		{
			//SE
			coll->CharacterOnceSE("CARankMax");
			m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kCA3Name);
			//段階ごとにエフェクトを変更
			m_levelEff.lock()->Delete();
			m_levelEff = EffekseerManager::GetInstance().CreateTrackActorEffect(std::string("ChargeLevel3Eff"), m_owner);
		}
	}
	//ボタンを離す
	else
	{
		ChangeState(std::make_shared<PlayerStateCA>(m_owner, actorManager, m_attackData));
		return;
	}
}
