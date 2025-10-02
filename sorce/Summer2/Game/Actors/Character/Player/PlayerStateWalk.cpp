#include "PlayerStateWalk.h"
#include "PlayerStateIdle.h"
#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
#include "PlayerStateNA.h"
#include "PlayerStateRolling.h"
#include "PlayerStateRun.h"
#include "PlayerStateCharge.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "PlayerStateWin.h"
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
#include "../../../../Game/Camera/GameCamera/GameCamera.h"


namespace
{
	//移動速度
	constexpr float kLowMoveSpeed = 2.0f;//地上の小移動速度
	constexpr float kMediumMoveSpeed = 5.0f;//地上の中移動速度
	constexpr float kHighMoveSpeed = 10.0f;//地上の大移動速度
	//アニメーション
	const char* kAnim = "Player|Walk";
}

PlayerStateWalk::PlayerStateWalk(std::weak_ptr<Actor> player):
	PlayerStateBase(player)
{
	//歩き状態
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Move);
}

PlayerStateWalk::~PlayerStateWalk()
{
}
void PlayerStateWalk::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateWalk::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto& input = Input::GetInstance();
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//ボスが完全に消滅したとき
	if (actorManager.lock()->IsBossDisappear())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//ボスの体力がなくなった場合または入力がないなら
	if (actorManager.lock()->IsBossDead() ||
		!input.GetStickInfo().IsLeftStickInput())
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
	//落下しているかチェック
	if (coll->GetRb()->GetVec().y <= Gravity::kChangeStateFall)
	{
		//落下
		ChangeState(std::make_shared<PlayerStateFall>(m_owner));
		return;
	}
	//回避ボタンを押したら
	if (input.IsTrigger("A"))
	{
		//回避
		ChangeState(std::make_shared<PlayerStateRolling>(m_owner));
		return;
	}
	//ゲージがあるとき使える
	if (input.IsTrigger("RB") && coll->GetUltGage().lock()->IsMaxUlt())
	{
		//必殺技
		ChangeState(std::make_shared<PlayerStateUltimate>(m_owner, actorManager));
		return;
	}
	//ジャンプボタンを押してるならジャンプ
	if (input.IsTrigger("B") && coll->IsFloor())
	{
		//ジャンプ
		ChangeState(std::make_shared<PlayerStateJump>(m_owner));
		return;
	}
	if (input.IsTrigger("X"))
	{
		//攻撃
		ChangeState(std::make_shared<PlayerStateNA>(m_owner, actorManager));
		return;
	}
	if (input.IsTrigger("Y"))
	{
		//チャージ
		ChangeState(std::make_shared<PlayerStateCharge>(m_owner, actorManager));
		return;
	}
	//ダッシュ
	if (input.IsPress("LS") || coll->IsRunKeep())
	{
		ChangeState(std::make_shared<PlayerStateRun>(m_owner));
		return;
	}

	auto rb = coll->GetRb();
	//移動
	rb->SetMoveVec(GetForwardVec(camera) * InputValueSpeed(input));
	//向きの更新
	Vector2 dir = coll->GetPlayerStickVec();
	coll->GetModel()->SetDir(dir);
}


float PlayerStateWalk::InputValueSpeed(const Input& input)
{
	float moveSpeed = 0.0f;
	//速度をスティック入力の深度に合わせる
	if (input.IsLowPowerLeftStick())
	{
		moveSpeed = kLowMoveSpeed;
	}
	else if (input.IsMediumPowerLeftStick())
	{
		moveSpeed = kMediumMoveSpeed;
	}
	else if (input.IsHighPowerLeftStick())
	{
		moveSpeed = kHighMoveSpeed;
	}
	return moveSpeed;
}