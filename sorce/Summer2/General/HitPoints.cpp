#include "HitPoints.h"
#include "../Game/Actors/Attack/AttackBase.h"
#include "Math/MyMath.h"

HitPoints::HitPoints() :
	m_hp(0),
	m_maxHp(0),
	m_isHit(false),
	m_isHitReaction(false),
	m_isNoDamage(false),
	m_buffCountFrame(0)
{
}

HitPoints::~HitPoints()
{
}

void HitPoints::Update()
{
	if (m_buffCountFrame < 0)
	{
		//バフが切れた時の処理
		if (m_initDefence.armor != m_defence.armor ||
			m_initDefence.damageCutRate != m_defence.damageCutRate )
		{
			m_defence = m_initDefence;
		}
	}
	else
	{
		//バフのフレーム
		--m_buffCountFrame;
	}
	ResetHitFlags();
}

void HitPoints::ResetHitFlags()
{
	m_isHit = false;
	m_isHitReaction = false;
}

void HitPoints::SetHp(int hp)
{
	m_hp = hp;
	m_maxHp = hp;
}

void HitPoints::Heal(int heal)
{
	if (m_hp >= m_maxHp)return;	//最大なら
	if (heal < 0)heal *= -1;
	m_hp += heal;
	m_hp = MathSub::ClampInt(m_hp, 0, m_maxHp);
}

void HitPoints::Damage(int damage)
{
	if (m_isNoDamage)return;	//無敵なら
	if (damage < 0)damage *= -1;
	m_hp -= static_cast<int>(static_cast<float>(damage) * m_defence.damageCutRate);
	if (m_hp <= 0)
	{
		m_hp = 0;
	}
}

void HitPoints::SetArmor(Battle::Armor am)
{
	m_defence.armor = am;
	if (m_buffCountFrame < 0)
	{
		m_initDefence = m_defence;
	}
}

void HitPoints::AddArmor(int am)
{
	m_defence.armor = m_defence.armor + am;
	if (m_buffCountFrame < 0)
	{
		m_initDefence.armor = m_defence.armor;
	}
}

void HitPoints::DefenseBuff(Battle::Armor armor, float damageCutRate, int buffFrame)
{
	m_defence.armor = armor;
	m_defence.damageCutRate = damageCutRate;
	m_buffCountFrame = buffFrame;
}

void HitPoints::Revival()
{
	m_hp = m_maxHp;
}
