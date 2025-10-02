#pragma once
#include "PlayerUIBase.h"
#include <memory>
class Player;
class PlayerHPUI :
    public PlayerUIBase
{
public:
    PlayerHPUI(std::weak_ptr<Player> player);
    ~PlayerHPUI();
    void Update()override;
    void Draw() const override;
private:
    float m_viewHp;
    float m_viewMaxHp;
    //HPÉtÉåÅ[ÉÄ
	int m_hpFrameHandle;
    //HP
	int m_hpHandle;
};

