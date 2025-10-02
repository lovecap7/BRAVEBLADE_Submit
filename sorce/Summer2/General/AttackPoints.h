#pragma once
#include "Battle.h"
class AttackPoints
{
public:
	AttackPoints();
	~AttackPoints();
	void Update();
	//�U����
	void SetAttackPower(int ap) { m_attack.attackPower = m_initAttack.attackPower = ap; };
	int GetAttackPower()const { return m_attack.attackPower; };
	//�_���[�W�{��
	void SetDamageRate(float rate) { m_attack.damageRate = m_initAttack.damageRate = rate;};
	float GetDamageRate() {return m_attack.damageRate; };
	//�U���̏d���̍Œ�l
	void SetLowestAW(Battle::AttackWeight aw) { m_attack.lowestAttackWeight = m_initAttack.lowestAttackWeight = aw; };
	Battle::AttackWeight GetLowestAW() { return	m_attack.lowestAttackWeight; };
	//�U���̃o�t���󂯂���
	void AttackBuff(float damageRate, Battle::AttackWeight lowestAttackWeight, int buffFrame);
private:
	struct Attack
	{
		//�U����
		int attackPower = 0;
		//�_���[�W�{��
		float damageRate = 1.0f;
		//�U���̏d���̍Œ�l
		Battle::AttackWeight lowestAttackWeight = Battle::AttackWeight::Light;
	};
	Attack m_attack;
	Attack m_initAttack;
	//�o�t�̌��ʎ���
	int m_buffCountFrame;
};

