#include "PlayerUIBase.h"

PlayerUIBase::PlayerUIBase(std::weak_ptr<Player> player):
	UIBase(true,UIBase::Priority::High),
	m_player(player)
{
}

PlayerUIBase::~PlayerUIBase()
{
}
