#pragma once
#include "../UIBase.h"
#include <memory>
class EnemyBase;
class EnemyUIBase:
	public UIBase
{
public:
    EnemyUIBase(std::weak_ptr<EnemyBase> enemy);
    ~EnemyUIBase();
protected:
    std::weak_ptr<EnemyBase> m_enemy;
};

