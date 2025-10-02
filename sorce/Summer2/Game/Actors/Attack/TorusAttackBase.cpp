#include "TorusAttackBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/TorusCollider.h"

TorusAttackBase::TorusAttackBase(std::weak_ptr<Actor> owner):
	AttackBase(Shape::Torus, owner)
{
}
void TorusAttackBase::SetPos(const Vector3& pos)
{
	m_rb->m_pos = pos;
}

void TorusAttackBase::SetRadius(float radius)
{
	std::dynamic_pointer_cast<TorusCollider>(m_collisionData)->SetRadius(radius);
}

void TorusAttackBase::SetRange(float range)
{
	std::dynamic_pointer_cast<TorusCollider>(m_collisionData)->SetRange(range);
}
