#include "BossKingStateDeath.h"
#include "BossKing.h"
#include "../EnemyBase.h"
#include "../../../Actor.h"
#include "../../../ActorManager.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../../../General/Sound/SoundManager.h"
#include "../../../../../General/Effect/EffekseerManager.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーション
	const char* kAnim = "Boss3|Boss3|Boss3|Dead";
	//高さ調整
	constexpr float kModelHeightAdjust = 1.0f;
}

BossKingStateDeath::BossKingStateDeath(std::weak_ptr<Actor> owner) :
	BossKingStateBase(owner,true)
{
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	//死亡状態にする
	coll->SetCollState(CollisionState::Dead);
	//死亡
	coll->GetModel()->SetAnim(kAnim, false);
	//無敵
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
	//BGMを止める
	SoundManager::GetInstance().StopBGM();
	//エフェクトの削除
	EffekseerManager::GetInstance().Reset();
	//高さ調整
	coll->GetModel()->SetModelHeightAdjust(kModelHeightAdjust);
	//VC
	coll->CharacterVC("Dead");
}

BossKingStateDeath::~BossKingStateDeath()
{
}

void BossKingStateDeath::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossKingStateDeath::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		coll->Delete();//削除
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
