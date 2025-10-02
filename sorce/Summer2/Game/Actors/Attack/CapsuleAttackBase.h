#pragma once
#include "AttackBase.h"
class Actor;
class CharacterStateBase;
class CapsuleAttackBase abstract:
    public AttackBase
{
public:
	CapsuleAttackBase(std::weak_ptr<Actor> owner);
	virtual ~CapsuleAttackBase() {};

	//始点
	void SetStartPos(const Vector3& pos);
	//終点
	void SetEndPos(const Vector3& pos);
	//半径
	void SetRadius(float radius);
protected:
};

