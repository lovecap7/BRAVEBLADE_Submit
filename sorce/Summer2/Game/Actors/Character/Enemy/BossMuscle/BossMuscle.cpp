#include "BossMuscle.h"
#include "BossMuscleStateIdle.h"
#include <memory>
#include <cassert>
#include "../../../ActorManager.h"
#include "../../Player/Player.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Input.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Collision/SphereCollider.h"
#include "../../../../../General/game.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../General/AttackPoints.h"
#include "../../../../../General/Sound/SoundManager.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../GameRule/Score.h"
#include "../../../Character/CharacterStateBase.h"
#include "../../../../UI/UIManager.h"
#include "../../../../UI/EnemyUI/BossHPUI.h"

namespace
{
	//当たり判定
	const Vector3 kCapsuleHeight = { 0.0f,150.0f,0.0f };//カプセルの上端
	constexpr float kCapsuleRadius = 80.0f; //カプセルの半径
	//モデルの高さ調整
	constexpr float kModelHeightAdjust = -70.0f;
	//名前
	const std::wstring kName = L"マッスル・レン";
}

BossMuscle::BossMuscle(int modelHandle, Vector3 pos) :
	EnemyBase(Shape::Capsule, EnemyGrade::Boss)
{
	//モデルの初期化
	m_model = std::make_unique<Model>(modelHandle, pos.ToDxLibVector());
	//衝突判定
	Vector3 endPos = pos;
	endPos += kCapsuleHeight; //カプセルの上端
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	cap->SetRadius(kCapsuleRadius);
	cap->SetEndPos(endPos);
	//リジッドボディの初期化
	m_rb->SetPos(pos);
	//体力ステータス
	m_hitPoints = std::make_shared<HitPoints>();
	//攻撃ステータス
	m_attackPoints = std::make_shared<AttackPoints>();
	//モデルの高さ調整
	m_model->SetModelHeightAdjust(kModelHeightAdjust);
}

BossMuscle::~BossMuscle()
{
}

void BossMuscle::Init()
{
	//コライダブルの初期化
	AllSetting(CollisionState::Normal, Priority::High, GameTag::Enemy, false, false, true);
	//Physicsに登録
	Collidable::Init();
	//サウンド
	InitSound();
	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = std::dynamic_pointer_cast<BossMuscle>(shared_from_this());
	m_state = std::make_shared<BossMuscleStateIdle>(thisPointer,false);
	//状態を変化する
	m_state->ChangeState(m_state);
	//名前
	m_name = kName;
}

void BossMuscle::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void BossMuscle::Complete()
{
	m_rb->m_pos = m_rb->GetNextPos();//次の座標へ
	Vector3 endPos = m_rb->m_pos;
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//カプセルの移動
	//モデルの座標更新
	m_model->SetPos(m_rb->GetPos().ToDxLibVector());
}

void BossMuscle::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	if (!m_hitPoints->IsDead())return;//体力がなくなっていない場合は無視
	//スコア加算
	score.lock()->AddKillOrItemScore(ScoreDataName::kBossMuscle);
	//死亡エフェクト
	EffekseerManager::GetInstance().CreateEffect("BossDeathEff", m_rb->m_pos);
	//爆発SE
	SoundManager::GetInstance().PlayOnceSE("BossBlast");
}

void BossMuscle::End()
{
	EndSound();
	m_model->End();
	Collidable::End();
}
void BossMuscle::InitSound()
{
	//VC
	m_soundHandles["Attack"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossMuscle/Attack.mp3");
	m_soundHandles["Damage"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossMuscle/Damage.mp3");
	m_soundHandles["Dead"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossMuscle/Dead.mp3");
	m_soundHandles["Angry"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossMuscle/Angry.mp3");
	//ロードに成功したかチェック
	for (const auto& [key, value] : m_soundHandles) {
		assert(value >= 0);
	}
}