#include "PlayerStateRolling.h"
#include "PlayerStateIdle.h"
#include "PlayerStateWin.h"
#include "PlayerStateHit.h"
#include "Player.h"
#include "../Enemy/EnemyBase.h"
#include "../../../../General/game.h"
#include "../../ActorManager.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Effect/TrackActorEffect.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../Game/Camera/GameCamera/GameCamera.h"

namespace
{
	//回避速度
	constexpr float kRollingMoveSpeed = 10.0f;
	//アニメーション
	const char* kAnim = "Player|Rolling";
	//回避モーションの速度
	constexpr float kAnimSpeed = 0.8f;
	//回避状態のフレーム数
	constexpr int kRollingFrame = 16;
}

PlayerStateRolling::PlayerStateRolling(std::weak_ptr<Actor> player) :
	PlayerStateBase(player)
{
	//回避状態
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	coll->SetCollState(CollisionState::Normal);
	//向きの更新
	coll->GetModel()->SetDir(coll->GetPlayerStickVec());
	//無敵
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
	//ランダムVC
	RandRollingVC(coll);
}

PlayerStateRolling::~PlayerStateRolling()
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//無敵解除
	coll->GetHitPoints().lock()->SetIsNoDamege(false);
}
void PlayerStateRolling::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
	//エフェクト
	EffekseerManager::GetInstance().CreateTrackActorEffect("RollingEff", m_owner);
	EffekseerManager::GetInstance().CreateEffect("LowShockWaveEff", m_owner.lock()->GetPos());
}

void PlayerStateRolling::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
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
	//攻撃を受けたなら
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_owner));
		return;
	}
	//無敵フレーム終了後解除
	if(coll->GetModel()->GetNowAnimFrame() >= kRollingFrame)
	{
		coll->GetHitPoints().lock()->SetIsNoDamege(false);
	}

	auto rb = coll->GetRb();
	//向いてる方向に移動
	rb->SetMoveVec(coll->GetModel()->GetDir() * kRollingMoveSpeed);
}

void PlayerStateRolling::RandRollingVC(std::shared_ptr<Player> coll)
{
	if (MyMath::IsRand())
	{
		coll->CharacterVC("Rolling1");
	}
	else
	{
		coll->CharacterVC("Rolling2");
	}
}