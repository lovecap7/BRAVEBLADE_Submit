#include "PlayerStateDeath.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "../../../../General/game.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../Camera/GameCamera/GameCamera.h"

namespace
{
	//アニメーション
	const char* kAnim = "Player|Dead";
	//減速率
	constexpr float kMoveDeceRate = 0.95f;
}

PlayerStateDeath::PlayerStateDeath(std::weak_ptr<Actor> player) :
	PlayerStateBase(player)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//死亡状態
	coll->GetModel()->SetAnim(kAnim, false);
	coll->SetCollState(CollisionState::Dead);
	//無敵
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
	//VC
	coll->CharacterVC("Dead");
}

PlayerStateDeath::~PlayerStateDeath()
{
}

void PlayerStateDeath::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateDeath::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//もしも体力が回復したら待機状態に戻る
	if (coll->GetHitPoints().expired())return;
	if (!coll->GetHitPoints().lock()->IsDead())
	{
		//無敵解除
		coll->GetHitPoints().lock()->SetIsNoDamege(false);
		//復活エフェクト
		EffekseerManager::GetInstance().CreateTrackActorEffect("RevivalPlayerEff", coll);
		//待機状態に戻る
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		//死亡
		coll->SetIsDead(true);
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

