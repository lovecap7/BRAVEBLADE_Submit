#include "SphereAttackBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/SphereCollider.h"

SphereAttackBase::SphereAttackBase(std::weak_ptr<Actor> owner):
	AttackBase(Shape::Sphere, owner)
{
}

void SphereAttackBase::SetPos(const Vector3& pos)
{
	m_rb->m_pos = pos;
}

void SphereAttackBase::SetRadius(float radius)
{
	std::dynamic_pointer_cast<SphereCollider>(m_collisionData)->SetRadius(radius);
}
