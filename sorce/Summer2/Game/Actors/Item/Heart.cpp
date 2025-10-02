#include "Heart.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/HitPoints.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Sound/SoundManager.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../../GameRule/Score.h"
namespace
{
	//回復量
	constexpr float kHealValue = 400.0f;
	//ジャンプ力
	constexpr float kJumpPower = 10.0f;
	//当たり判定の半径
	constexpr float kCollRadius = 80.0f;
}

Heart::Heart(int modelHandle, Vector3 pos):
	ItemBase(Shape::Sphere)
{
	//座標
	auto firstPos = pos;
	firstPos.y += kCollRadius;
	m_rb->m_pos = firstPos;
	std::dynamic_pointer_cast<SphereCollider>(m_collisionData)->SetRadius(kCollRadius);
	//モデル
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//力を与える
	m_rb->SetVecY(kJumpPower);
}

Heart::~Heart()
{
}

void Heart::OnCollide(const std::shared_ptr<Collidable> other)
{
	//消滅フラグが立ってるならリターン
	if (m_isDelete)return;
	//移動量を初期化
	m_rb->SetVec(Vector3::Zero());
	//プレイヤーに当たった時の処理
	if (other->GetGameTag() == GameTag::Player)
	{
		//SE
		SoundManager::GetInstance().PlayOnceSE("GetItem");
		//回復
		auto player = std::dynamic_pointer_cast<Player>(other);
		player->GetHitPoints().lock()->Heal(kHealValue);
		//回復エフェクト
		EffekseerManager::GetInstance().CreateTrackActorEffect("GetHealEff", player);
		//削除
		m_isDelete = true;
	}
}

void Heart::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	//スコア加算
	score.lock()->AddKillOrItemScore(ScoreDataName::kHeart);
}
