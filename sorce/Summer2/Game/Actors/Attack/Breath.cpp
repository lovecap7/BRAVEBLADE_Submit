#include "Breath.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../Character/CharacterBase.h"
#include "../../../General/HitPoints.h"
#include "../../Camera/GameCamera/GameCamera.h"
#include "../Character/Player/Player.h"
#include "../Character/Player/UltGage.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Sound/SoundManager.h"

Breath::Breath(std::weak_ptr<Actor> owner) :
	SphereAttackBase(owner)
{
	//SE
	SoundManager::GetInstance().PlayOnceSE("Shot");
}

void Breath::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//共通の処理をする
	AttackBase::Update(actorManager);
	//移動
	m_rb->m_pos = m_rb->GetNextPos();
}


void Breath::OnCollide(const std::shared_ptr<Collidable> other)
{
	AttackBase::OnCollide(other);
	//攻撃が成功したなら
	if (m_isSuccessAttack)
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
		//ヒットエフェクト
		auto HitPos = m_rb->m_pos;
		EffekseerManager::GetInstance().CreateEffect("ImpactHitEff", HitPos);
	}
}


void Breath::Draw() const
{
	auto coll = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
	DrawSphere3D(m_rb->m_pos.ToDxLibVector(),
		coll->GetRadius(), 16, 0xff0000, 0xffffff, true);
}


void Breath::SetVec(Vector3 vec)
{
	//移動量
	m_rb->m_vec = vec;
}