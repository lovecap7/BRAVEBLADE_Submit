#include "BossKingStateStart.h"
#include "BossKingStateIdle.h"
#include "BossKing.h"
#include "../EnemyBase.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../ActorManager.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../../General/Sound/SoundManager.h"
namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーション
	const char* kAnim = "Boss3|Start";
}

BossKingStateStart::BossKingStateStart(std::weak_ptr<Actor> owner) :
	BossKingStateBase(owner,false)
{
	//スタート状態
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, false);
	coll->SetCollState(CollisionState::Normal);
	//オーラエフェクト
	EffekseerManager::GetInstance().CreateTrackActorEffect("BossKingStartEff", m_owner);
}

BossKingStateStart::~BossKingStateStart()
{
	
}

void BossKingStateStart::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void BossKingStateStart::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//入力を止める
	auto& input = Input::GetInstance();
	input.StopUpdate();
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	//モデルのアニメーションが終わったら
	if (coll->GetModel()->IsFinishAnim())
	{
		//入力を開始
		input.StartUpdate();
		//待機
		ChangeState(std::make_shared<BossKingStateIdle>(m_owner,false));
		return;
	}
	//セリフと風
	if (coll->GetModel()->GetNowAnimFrame() == coll->GetModel()->GetTotalAnimFrame() * 0.25f)
	{
		//VC
		coll->CharacterVC("Start");
		//SE
		SoundManager::GetInstance().PlayOnceSE("Wind");
	}
	//爆発
	else if (coll->GetModel()->GetNowAnimFrame() == coll->GetModel()->GetTotalAnimFrame() * 0.5f)
	{
		//SE
		SoundManager::GetInstance().PlayOnceSE("Blast");
	}

	//だんだん減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
