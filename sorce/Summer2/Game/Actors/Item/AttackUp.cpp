#include "AttackUp.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/AttackPoints.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Effect/TrackActorEffect.h"
#include "../../../General/Sound/SoundManager.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../../GameRule/Score.h"

namespace
{
	//ダメージ倍率
	constexpr float kDamageRate = 1.5f;
	//持続フレーム
	constexpr int kAttackUpKeepFrame = 10 * 60;
	//ジャンプ力
	constexpr float kJumpPower = 10.0f;
	//当たり判定の半径
	constexpr float kCollRadius = 80.0f;
}

AttackUp::AttackUp(int modelHandle, Vector3 pos) :
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

AttackUp::~AttackUp()
{
}

void AttackUp::OnCollide(const std::shared_ptr<Collidable> other)
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
		//攻撃アップバフ
		auto player = std::dynamic_pointer_cast<Player>(other);
		player->GetAttackPoints().lock()->AttackBuff(kDamageRate, Battle::AttackWeight::Heavy, kAttackUpKeepFrame);
		//パワーアップエフェクト
		EffekseerManager::GetInstance().CreateTrackActorEffect("GetAttackUpEff", player);
		auto eff = EffekseerManager::GetInstance().CreateTrackActorEffect("AttackUpEff", player);
		eff.lock()->SpecificFrame(kAttackUpKeepFrame);
		//削除
		m_isDelete = true;
	}
}

void AttackUp::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	//スコア加算
	score.lock()->AddKillOrItemScore(ScoreDataName::kAttackUp);
}
