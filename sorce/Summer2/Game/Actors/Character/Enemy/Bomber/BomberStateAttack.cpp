#include "BomberStateAttack.h"
#include "BomberStateDeath.h"
#include "BomberStateHit.h"
#include "BomberStateIdle.h"
#include "Bomber.h"
#include "../EnemyBase.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../ActorManager.h"
#include "../../../Item/Bomb.h"
namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//左腕と左手のインデックス
	constexpr int kLeftHandIndex = 17;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 150;//2.5秒くらいの感覚で攻撃
	//爆弾の上昇量
	constexpr float kBombUpVecY = 10.0f; //爆弾の上昇量
	//爆弾の移動量
	constexpr float kBombMoveVecPower = 3.0f; //爆弾の移動量
}

BomberStateAttack::BomberStateAttack(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager) :
	BomberStateBase(owner),
	m_attackCountFrame(0)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kAttackName);
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	//通常攻撃
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//相手のほうを向く
	coll->LookAtTarget();
	//VC
	coll->CharacterVC("Attack");
}

BomberStateAttack::~BomberStateAttack()
{
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
	//グループに所属しているなら攻撃権を消す
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BomberStateAttack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BomberStateAttack::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BomberStateDeath>(m_owner));
		return;
	}
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BomberStateHit>(m_owner));
		return;
	}
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		CreateBomb(actorManager);
	}
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		//待機状態に戻す
		ChangeState(std::make_shared<BomberStateIdle>(m_owner));
		return;
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BomberStateAttack::CreateBomb(const std::weak_ptr<ActorManager> actorManager)
{
	auto model = m_owner.lock()->GetModel();
	//生成位置
	VECTOR leftHand = MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex);//手の指先
	leftHand = VAdd(leftHand, VScale(model->GetDir().ToDxLibVector(), 20.0f));//少し前に出す
	auto item = actorManager.lock()->CreateItem(ItemType::Bomb, leftHand).lock();
	//移動量
	Vector3 moveVec = model->GetDir();
	moveVec *= kBombMoveVecPower;
	moveVec.y = kBombUpVecY; //上昇量を追加
	std::dynamic_pointer_cast<Bomb>(item)->SetVec(moveVec);
}