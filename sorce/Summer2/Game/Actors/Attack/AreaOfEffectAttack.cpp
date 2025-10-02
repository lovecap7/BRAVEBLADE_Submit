#include "AreaOfEffectAttack.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Effect/MyEffect.h"
#include "../../../General/Sound/SoundManager.h"
#include "../../Camera/GameCamera/GameCamera.h"
#include "../Character/CharacterBase.h"

AreaOfEffectAttack::AreaOfEffectAttack(std::weak_ptr<Actor> owner):
	SphereAttackBase(owner)
{
}

AreaOfEffectAttack::~AreaOfEffectAttack()
{
}

void AreaOfEffectAttack::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	AttackBase::Update(actorManager);
}

void AreaOfEffectAttack::Draw() const
{
#if _DEBUG
	auto coll = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
	DrawSphere3D(m_rb->m_pos.ToDxLibVector(),
		coll->GetRadius(), 16, 0xff0000, 0xff0000, false);
#endif
}
void AreaOfEffectAttack::OnCollide(const std::shared_ptr<Collidable> other)
{
	AttackBase::OnCollide(other);
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
		auto otherPos = std::dynamic_pointer_cast<CharacterBase>(other)->GetPos();
		auto centerPos = (otherPos + m_rb->m_pos) / 2.0f;
		//ヒットエフェクト
		auto eff = EffekseerManager::GetInstance().CreateEffect("ImpactHitEff", centerPos);
	}
}
