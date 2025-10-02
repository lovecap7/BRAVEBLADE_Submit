#include "Slash.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Effect/MyEffect.h"
#include "../../../General/Sound/SoundManager.h"
#include "../ActorManager.h"
#include "../../Camera/GameCamera/GameCamera.h"

Slash::Slash(std::weak_ptr<Actor> owner):
	CapsuleAttackBase(owner)
{
}

Slash::~Slash()
{
}

void Slash::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	AttackBase::Update(actorManager);
}

void Slash::Draw() const
{
#if _DEBUG
	auto coll = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	DrawCapsule3D(m_rb->m_pos.ToDxLibVector(), coll->GetEndPos().ToDxLibVector(),
		coll->GetRadius(), 16, 0xff0000, 0xff0000, false);
#endif
}

void Slash::OnCollide(const std::shared_ptr<Collidable> other)
{
	AttackBase::OnCollide(other);
	//攻撃が成功したなら
	if (m_isSuccessAttack)
	{
		//SE
		if (MyMath::IsRand())
		{	
			SoundManager::GetInstance().PlayOnceSE("SlashHit1");
		}
		else
		{
			SoundManager::GetInstance().PlayOnceSE("SlashHit2");
		}
		//ヒットエフェクト
		auto coll = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
		auto HitPos = (m_rb->m_pos + coll->GetEndPos()) / 2.0f;
		auto eff = EffekseerManager::GetInstance().CreateEffect("SlashHitEff", HitPos);
	}
}
