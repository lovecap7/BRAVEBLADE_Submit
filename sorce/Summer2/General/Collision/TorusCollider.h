#pragma once
#include "ColliderBase.h"
#include "../Math/MyMath.h"
class TorusCollider :
    public ColliderBase
{
public:
	TorusCollider();
    virtual ~TorusCollider() {};
    //�͈͎擾
	float GetRange() const { return m_range; }
	//�͈͐ݒ�
	void  SetRange(float range)  { m_range = range; }
	//���a�擾
	float GetRadius() const { return m_radius; }
	//���a�ݒ�
	void  SetRadius(float radius) { m_radius = radius; }
private:
    //�͈�
	float m_range;
    //���a
	float m_radius;
};

