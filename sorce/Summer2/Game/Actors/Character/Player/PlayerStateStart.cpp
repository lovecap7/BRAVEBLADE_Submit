#include "PlayerStateStart.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../Game/UI/UIManager.h"
namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//全体フレーム
	constexpr float kTotalAnimFrame = 37.0f;
	//アニメーション
	const char* kAnim = "Player|Ult";
}

PlayerStateStart::PlayerStateStart(std::weak_ptr<Actor> player) :
	PlayerStateBase(player)
{
	//スタート状態
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, false);
	coll->SetCollState(CollisionState::Normal);
	//全体フレーム設定
	coll->GetModel()->SetTotalAnimFrame(kTotalAnimFrame);
	//UI非表示
	UIManager::GetInstance().StopDraw();
	UIManager::GetInstance().StopUpdate();
}

PlayerStateStart::~PlayerStateStart()
{
	//UI表示
	UIManager::GetInstance().StartDraw();
	UIManager::GetInstance().StartUpdate();
}

void PlayerStateStart::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateStart::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//入力を止める
	auto& input = Input::GetInstance();
	input.StopUpdate();
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto model = coll->GetModel();
	//モデルのアニメーションが終わったら
	if (model->IsFinishAnim())
	{
		//入力を開始
		input.StartUpdate();
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	if (model->GetNowAnimFrame() == model->GetTotalAnimFrame() / 2.0f)
	{
		//VC
		coll->CharacterVC("Start");
	}
	//だんだん減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
