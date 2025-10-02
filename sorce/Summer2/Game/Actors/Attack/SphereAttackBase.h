#pragma once
#include "AttackBase.h"
class Actor;
class CharacterStateBase;
class SphereAttackBase abstract :
	public AttackBase
{
public:
	SphereAttackBase(std::weak_ptr<Actor> owner);
	virtual ~SphereAttackBase() {};
	//’†“_
	void SetPos(const Vector3& pos);
	//”¼Œa
	void SetRadius(float radius);
protected:
};
