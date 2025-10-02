#include "Bomb.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../Character/Player/UltGage.h"
#include "../ActorManager.h"
#include "../Attack/Blast.h"
#include "../Character/Player/Player.h"
#include "../../GameRule/Score.h"
namespace
{
	//爆発までのフレーム数
	constexpr int kBlastFrame = 180;
	//ジャンプ力
	constexpr float kJumpPower = 10.0f;
	//当たり判定の半径
	constexpr float kCollRadius = 50.0f;
	//爆発寸前のフレーム
	constexpr int kBeforeBlastFrame = 60;
	//モデルの大きさ
	const VECTOR kModelScale = { 0.7f, 0.7f, 0.7f };
	//拡大縮小の大きさ
	const Vector3 kAddScaleSize = { 0.1f, 0.1f, 0.1f };
	//モデルの拡大縮小のスピード
	constexpr float kSlowScaleSpeed = 10.0f;
	constexpr float kFastScaleSpeed = 25.0f;

	//攻撃のデータを検索するときに使う
	std::string kOwnerName = "Bomb";
	std::string kAttackName = "Bomber";
}

Bomb::Bomb(int modelHandle, Vector3 pos) :
	ItemBase(Shape::Sphere),
	m_blastCountFrame(kBlastFrame),
	m_scaleSpeed(0.0f),
	m_originalScale(kModelScale)
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

Bomb::~Bomb()
{
}

void Bomb::Init()
{
	//コライダブルの初期化
	AllSetting(CollisionState::Normal, Priority::Low, GameTag::Item, false, false, true);
	//Physicsに登録
	Collidable::Init();
}

void Bomb::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//爆発までのカウント
	if (m_blastCountFrame > 0)
	{
		--m_blastCountFrame;
		//爆弾の自作アニメーション
		AnimBomb();
	}
	else
	{
		//消滅
		m_isDelete = true;
	}
	m_model->Update();
}


void Bomb::OnCollide(const std::shared_ptr<Collidable> other)
{
	//移動量を初期化
	m_rb->SetVec(Vector3::Zero());
}

void Bomb::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	//攻撃データ
	auto data = actorManager.lock()->GetAttackData(kOwnerName, kAttackName);
	//爆発の攻撃判定を出す
	auto attack = actorManager.lock()->CreateAttack(AttackType::Blast, std::dynamic_pointer_cast<Actor>(shared_from_this()));
	auto blast = std::dynamic_pointer_cast<Blast> (attack.lock());
	blast->SetPos(m_rb->m_pos);
	//大きさ
	blast->SetRadius(data.radius);
	//ダメージ、持続フレーム、ノックバックの大きさ、攻撃の重さ、ヒットストップの長さ、カメラの揺れ
	blast->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
}

void Bomb::SetVec(Vector3 vec)
{
	m_rb->m_vec = vec;
}

void Bomb::AnimBomb()
{
	if (m_blastCountFrame <= kBeforeBlastFrame)
	{
		//爆発までの残り時間が短いとき早くなる
		m_scaleSpeed += kFastScaleSpeed;
	}
	else
	{
		//それ以外はゆっくり拡大縮小
		m_scaleSpeed += kSlowScaleSpeed;
	}
	Vector3 modelScale = m_originalScale + kAddScaleSize * cosf(m_scaleSpeed * MyMath::DEG_2_RAD);
	m_model->SetScale(modelScale.ToDxLibVector());
}