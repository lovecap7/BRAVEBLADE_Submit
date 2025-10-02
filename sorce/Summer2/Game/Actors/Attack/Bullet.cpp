#include "Bullet.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../Character/CharacterBase.h"
#include "../../../General/HitPoints.h"
#include "../../../General/Sound/SoundManager.h"
#include "../../Camera/GameCamera/GameCamera.h"
#include "../Character/Player/Player.h"
#include "../Character/Player/UltGage.h"
#include "../ActorManager.h"

Bullet::Bullet(std::weak_ptr<Actor> owner):
	SphereAttackBase(owner)
{
	//SE
	SoundManager::GetInstance().PlayOnceSE("Shot");
}

void Bullet::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//攻撃が当たったなら
	if (m_isHit)
	{
		//SE
		if (MyMath::IsRand())
		{
			SoundManager::GetInstance().PlayOnceSE("Hit1");
		}
		else
		{
			SoundManager::GetInstance().PlayOnceSE("Hit2");
		}
		//ヒットストップ
		actorManager.lock()->HitStop(m_shakePower, m_hitStopFrame);
		//削除
		m_isDelete = true;
		return;
	}
	//共通の処理をする
	AttackBase::Update(actorManager);
	//移動
	m_rb->m_pos = m_rb->GetNextPos();
}

void Bullet::OnCollide(const std::shared_ptr<Collidable> other)
{
	if (m_owner.expired())return;
	auto ownerColl = m_owner.lock();
	auto otherColl = other;

	//自分と同じ種類なら無視
	if (otherColl->GetGameTag() == ownerColl->GetGameTag())return;

	//プレイヤーか敵なら
	if (otherColl->GetGameTag() == GameTag::Player ||
		otherColl->GetGameTag() == GameTag::Enemy)
	{
		if (std::dynamic_pointer_cast<CharacterBase>(otherColl)->GetHitPoints().lock()->IsNoDamege())
		{
			//ダメージを受けない状態なら無視
			return;
		}
	}
	else
	{
		//それ以外のActorは無視
		return;
	}
	//プレイヤーの攻撃の場合必殺技ゲージを加算する
	if (ownerColl->GetGameTag() == GameTag::Player)
	{
		std::dynamic_pointer_cast<Player>(ownerColl)->GetUltGage().lock()->AddPedingUltGage();//予約されていた加算ゲージ量を反映
	}
	//攻撃を受けたときの処理
	std::dynamic_pointer_cast<CharacterBase>(otherColl)->OnHitFromAttack(shared_from_this());
	//ヒットエフェクト
	auto HitPos = m_rb->m_pos;
	EffekseerManager::GetInstance().CreateEffect("ImpactHitEff", HitPos);
	//攻撃を当てたので
	m_isHit = true;
}

void Bullet::Draw() const
{
	auto coll = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
	DrawSphere3D(m_rb->m_pos.ToDxLibVector(),
		coll->GetRadius(), 16, 0xff0000, 0xffffff, true);
}


void Bullet::SetVec(Vector3 vec)
{
	//移動量
	m_rb->m_vec = vec;
}
