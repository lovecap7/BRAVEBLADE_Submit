#pragma once
#include "../UIBase.h"
#include <memory>
class Player;
class PlayerUIBase :
    public UIBase
{
public:
    PlayerUIBase(std::weak_ptr<Player> player);
    ~PlayerUIBase();
protected:
    std::weak_ptr<Player> m_player;
};

