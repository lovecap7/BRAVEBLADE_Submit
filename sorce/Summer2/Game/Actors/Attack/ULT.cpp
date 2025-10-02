#include "ULT.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Effect/MyEffect.h"
#include "../../../General/Sound/SoundManager.h"
#include "../ActorManager.h"
#include "../../Camera/GameCamera/GameCamera.h"

ULT::ULT(std::weak_ptr<Actor> owner):
	Slash(owner)
{
}

ULT::~ULT()
{
}

void ULT::OnCollide(const std::shared_ptr<Collidable> other)
{
	AttackBase::OnCollide(other);
	if (m_isHit)
	{
		//SE
		SoundManager::GetInstance().PlayOnceSE("UltHit");
		//ヒットエフェクト
		auto coll = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
		auto eff = EffekseerManager::GetInstance().CreateEffect("UltHitEff", std::dynamic_pointer_cast<Actor>(other)->GetPos());
	}
}
