#include "ItemBase.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/HitPoints.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Effect/TrackActorEffect.h"
#include "../../../General/Sound/SoundManager.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../../GameRule/Score.h"

namespace
{
	//ダメージカット
	constexpr float kDamageCutRate = 0.5f;
	//持続フレーム
	constexpr int kDamageCutKeepFrame = 60 * 20;//15秒
	//ジャンプ力
	constexpr float kJumpPower = 10.0f;
	//当たり判定の半径
	constexpr float kCollRadius = 80.0f;
	//回転量
	constexpr float kRotaAngle = 1.0f;
	//最初の当たらないフレーム
	constexpr int kNoHitFrame = 30;
}

ItemBase::ItemBase(Shape shape):
	Actor(shape),
	m_noHitFrame(kNoHitFrame)
{
}

ItemBase::~ItemBase()
{
}

void ItemBase::Init()
{
	//コライダブルの初期化
	AllSetting(CollisionState::Normal, Priority::Low, GameTag::Item, true, false, true);
	//Physicsに登録
	Collidable::Init();
}

void ItemBase::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//移動量を初期化
	m_rb->SetMoveVec(Vector3::Zero());
	//回る
	m_model->SetRot(VGet(0.0f, kRotaAngle, 0.0f));
	if (m_noHitFrame > 0)
	{
		--m_noHitFrame;
	}
	else
	{
		//当たり判定をする
		m_isThrough = false;
	}
	m_model->Update();
}

void ItemBase::Draw() const
{
#if _DEBUG
	//衝突判定
	DrawSphere3D(
		GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<SphereCollider>(m_collisionData)->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		false
	);
#endif
	m_model->Draw();
}

void ItemBase::Complete()
{
	//次の座標へ
	m_rb->m_pos = m_rb->GetNextPos();
	//モデルの座標更新
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
}

void ItemBase::End()
{
	Collidable::End();
	m_model->End();
}
