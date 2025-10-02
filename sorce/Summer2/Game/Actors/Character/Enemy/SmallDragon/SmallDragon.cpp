#include "SmallDragon.h"
#include "SmallDragonStateBase.h"
#include "SmallDragonStateIdle.h"
#include "../../CharacterStateBase.h"
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
#include "../../../../UI/UIManager.h"
#include "../../../SearchPlace.h"
namespace
{
	//当たり判定
	const Vector3 kCapsuleHeight = { 0.0f,120.0f,0.0f };//カプセルの上端
	constexpr float kCapsuleRadius = 40.0f; //カプセルの半径
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//名前
	const std::wstring kName = L"レンドラゴン";
}
SmallDragon::SmallDragon(int modelHandle, Vector3 pos) :
	EnemyBase(Shape::Capsule,EnemyGrade::Normal)
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
	//索敵場所
	m_searchPlace = std::make_shared<SearchPlace>(GetPos(), 0.0f);
}

SmallDragon::~SmallDragon()
{
}

void SmallDragon::Init()
{
	//コライダブルの初期化
	AllSetting(CollisionState::Normal, Priority::Middle, GameTag::Enemy, false, false,true);
	//Physicsに登録
	Collidable::Init();
	//サウンド
	InitSound();
	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = std::dynamic_pointer_cast<SmallDragon>(shared_from_this());
	m_state = std::make_shared<SmallDragonStateIdle>(thisPointer);
	//状態を変化する
	m_state->ChangeState(m_state);
	//敵関連のUIの準備
	UIManager::GetInstance().CreateEnemyUI(thisPointer);
	//名前
	m_name = kName;
}

void SmallDragon::OnCollide(const std::shared_ptr<Collidable> other)
{

}

void SmallDragon::Complete()
{
	m_rb->m_pos = m_rb->GetNextPos();//次の座標へ
	Vector3 endPos = m_rb->m_pos;
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//カプセルの移動
	//モデルの座標更新
	m_model->SetPos(m_rb->GetPos().ToDxLibVector());
}

void SmallDragon::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	if (!m_hitPoints->IsDead())return;//体力がなくなっていない場合は無視
	//スコア加算
	score.lock()->AddKillOrItemScore(ScoreDataName::kSmallDragon);
	//アイテムをランダムで落とす
	auto actorM = actorManager.lock();
	if (MyMath::IsRand())
	{
		actorM->CreateRandItem(m_rb->GetPos());
	}
	//死亡エフェクト
	EffekseerManager::GetInstance().CreateEffect("DeathEff", m_rb->m_pos);
	//爆発SE
	SoundManager::GetInstance().PlayOnceSE("Blast");
}

void SmallDragon::End()
{
	EndSound();
	m_model->End();
	Collidable::End();
}

void SmallDragon::InitSound()
{
	//VC
	m_soundHandles["Attack"] = LoadSoundMem(L"Data/Sound/VC/Enemy/SmallDragon/Attack.mp3");
	m_soundHandles["Damage"] = LoadSoundMem(L"Data/Sound/VC/Enemy/SmallDragon/Damage.mp3");
	m_soundHandles["Dead"] = LoadSoundMem(L"Data/Sound/VC/Enemy/SmallDragon/Dead.mp3");
	//ロードに成功したかチェック
	for (const auto& [key, value] : m_soundHandles) {
		assert(value >= 0);
	}
}