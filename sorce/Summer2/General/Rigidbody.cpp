#include "Rigidbody.h"
#include "Math/Vector2.h"

namespace
{
	//0�ɋ߂��l
	constexpr float kEpsilon = 0.0001f;
}

Rigidbody::Rigidbody():
	m_pos(),
	m_vec(),
	m_isGravity(true) //�d�͂��󂯂�
{
}


Vector3 Rigidbody::GetMoveVec()
{
	Vector3 moveVec = m_vec;
	moveVec.y = 0.0f; //Y���͖���
	//X,Z�̃x�N�g����Ԃ�
	if (moveVec.Magnitude() <= 0.0f)
	{
		//�ړ��x�N�g����0�Ȃ�0��Ԃ�
		moveVec = Vector3{ 0.0f,0.0f,0.0f };
	}
	
	return moveVec;
}

void Rigidbody::SetMoveVec(Vector3 vec)
{
	m_vec.x = vec.x;
	m_vec.z = vec.z;
}

void Rigidbody::SpeedDown(float decRate)
{
	//����
	m_vec.x *= decRate;
	m_vec.z *= decRate;
	//�ق�0�Ȃ�
	if (m_vec.XZ().Magnitude() <= kEpsilon)
	{
		m_vec.x = 0.0f;
		m_vec.z = 0.0f;
	}
}
