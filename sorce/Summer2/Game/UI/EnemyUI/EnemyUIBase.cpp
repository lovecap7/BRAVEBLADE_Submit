#include "EnemyUIBase.h"

EnemyUIBase::EnemyUIBase(std::weak_ptr<EnemyBase> enemy):
	UIBase(),
	m_enemy(enemy)
{
}

EnemyUIBase::~EnemyUIBase()
{
}
