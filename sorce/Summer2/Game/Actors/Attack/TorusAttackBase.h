#pragma once
#include "AttackBase.h"
class TorusAttackBase abstract:
    public AttackBase
{
public:
	TorusAttackBase(std::weak_ptr<Actor> owner);
	virtual ~TorusAttackBase() {};
	//���_
	void SetPos(const Vector3& pos);
	//���a
	void SetRadius(float radius);
	//�͈�
	void SetRange(float range);
protected:
};

