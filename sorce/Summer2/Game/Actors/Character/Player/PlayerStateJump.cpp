#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "../Enemy/EnemyBase.h"
#include "../../ActorManager.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/HitPoints.h"
#include "../../../../Game/Camera/GameCamera/GameCamera.h"


namespace
{
	//ジャンプ力
	constexpr float kJumpPower = 15.0f;
	//空中移動速度
	constexpr float kMaxAirMoveSpeed = 10.00f;
	//アニメーション
	const char* kAnim = "Player|Jump";
	//移動速度
	constexpr float kLowAirMoveSpeed = 0.1f;//空中の小移動速度
	constexpr float kMediumAirMoveSpeed = 0.2f;//空中の中移動速度
	constexpr float kHighAirMoveSpeed = 0.5f;//空中の大移動速度
}

PlayerStateJump::PlayerStateJump(std::weak_ptr<Actor> player):
	PlayerStateBase(player)
{
	//ジャンプ
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Jump);
	//地面から離れるのでfalseにしておく
	coll->SetIsFloor(false);
	//力を与える
	coll->GetRb()->SetVecY(kJumpPower);
	//SE
	coll->CharacterOnceSE("Jump");
}

PlayerStateJump::~PlayerStateJump()
{
}
void PlayerStateJump::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateJump::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto& input = Input::GetInstance();
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto rb = coll->GetRb();
	//ボスを倒す
	if (actorManager.lock()->IsBossDisappear())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
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
	//落下しているなら
	if (rb->GetVec().y < 0.0f)
	{
		//落下
		ChangeState(std::make_shared<PlayerStateFall>(m_owner));
		return;
	}
	//移動の入力があるなら
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//空中移動
		rb->AddVec(GetForwardVec(camera) * InputValueSpeed(input));
		//横移動速度に上限をつける
		float speed = rb->GetMoveVec().Magnitude();
		if (speed > 0.0f)
		{
			speed = MathSub::ClampFloat(speed, 0.0f, kMaxAirMoveSpeed);
			Vector3 moveVec = rb->GetMoveVec();
			if (moveVec.Magnitude() > 0.0f)
			{
				moveVec = moveVec.Normalize();
			}
			rb->SetMoveVec(moveVec * speed);
		}
	}
	
	//向きの更新
	coll->GetModel()->SetDir(coll->GetPlayerStickVec());
}

float PlayerStateJump::InputValueSpeed(const Input& input)
{
	float moveSpeed = 0.0f;
	//速度をスティック入力の深度に合わせる
	if (input.IsLowPowerLeftStick())
	{
		moveSpeed = kLowAirMoveSpeed;
	}
	else if (input.IsMediumPowerLeftStick())
	{
		moveSpeed = kMediumAirMoveSpeed;
	}
	else if (input.IsHighPowerLeftStick())
	{
		moveSpeed = kHighAirMoveSpeed;
	}
	return moveSpeed;
}

