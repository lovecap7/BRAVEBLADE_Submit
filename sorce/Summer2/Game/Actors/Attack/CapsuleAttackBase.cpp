#include "CapsuleAttackBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/CapsuleCollider.h"

CapsuleAttackBase::CapsuleAttackBase(std::weak_ptr<Actor> owner) :
	AttackBase(Shape::Capsule, owner)
{
}

void CapsuleAttackBase::SetStartPos(const Vector3& pos)
{
	m_rb->m_pos = pos;
}

void CapsuleAttackBase::SetEndPos(const Vector3& pos)
{
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	cap->SetEndPos(pos);
}

void CapsuleAttackBase::SetRadius(float radius)
{
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	cap->SetRadius(radius);
}
