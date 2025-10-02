#include "BossKingStateChange.h"
#include "BossKingStateIdle.h"
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
#include "../../../../UI/UIManager.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーション
	const char* kDeathAnim = "Boss3|Death";
	const char* kStandingAnim = "Boss3|Standing";
	//アニメーション速度
	constexpr float kAnimSpeed = 0.3f;
}

BossKingStateChange::BossKingStateChange(std::weak_ptr<Actor> owner) :
	BossKingStateBase(owner,false)
{
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//死亡
	coll->GetModel()->SetAnim(kDeathAnim, false, kAnimSpeed);
	//無敵
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
	//VC
	coll->CharacterVC("Change");
	//SE
	coll->CharacterOnceSE("Change");
}

BossKingStateChange::~BossKingStateChange()
{
}

void BossKingStateChange::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
	//エフェクト
	EffekseerManager::GetInstance().CreateTrackActorEffect("BossKingTransEff", m_owner);
}

void BossKingStateChange::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//入力を止める
	auto& input = Input::GetInstance();
	input.StopUpdate();
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		if (m_isTransformSecond)
		{
			//開始
			input.StartUpdate();
			//UIの描画を開始
			UIManager::GetInstance().StartDraw();
			//全回復
			coll->FullRecovery();
			//無敵解除
			coll->GetHitPoints().lock()->SetIsNoDamege(false);
			//待機状態
			ChangeState(std::make_shared<BossKingStateIdle>(m_owner, true));
			return;
		}
		else
		{
			//SE
			coll->CharacterOnceSE("Thunder");
			//変身
			coll->TransformSecond();
			//立ち上がる
			coll->GetModel()->SetAnim(kStandingAnim, false);
			//変身フラグ
			m_isTransformSecond = true;
		}
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
