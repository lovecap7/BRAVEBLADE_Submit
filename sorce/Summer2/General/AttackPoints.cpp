#include "AttackPoints.h"

AttackPoints::AttackPoints():
	m_buffCountFrame(0)
{
	m_attack.damageRate = 1.0f;
	m_attack.lowestAttackWeight = Battle::AttackWeight::Light;
	m_initAttack = m_attack;
}

AttackPoints::~AttackPoints()
{
}

void AttackPoints::Update()
{
	if (m_buffCountFrame < 0)
	{
		//バフが切れた時の処理
		if (m_initAttack.damageRate != m_attack.damageRate ||
			m_initAttack.lowestAttackWeight != m_attack.lowestAttackWeight)
		{
			m_attack = m_initAttack;
		}
	}
	else
	{
		//バフのフレーム
		--m_buffCountFrame;
	}
}

void AttackPoints::AttackBuff(float damageRate, Battle::AttackWeight lowestAttackWeight, int buffFrame)
{
	m_attack.damageRate = damageRate;
	m_attack.lowestAttackWeight = lowestAttackWeight;
	m_buffCountFrame = buffFrame;
}
