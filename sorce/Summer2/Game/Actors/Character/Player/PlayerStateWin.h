#pragma once
#include "PlayerStateBase.h"
class Actor;
class Input;
class GameCamera;
class ActorManager;
class PlayerStateWin :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateWin>
{
public:
    PlayerStateWin(std::weak_ptr<Actor>  player);
    ~PlayerStateWin();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    int m_countFrame;
};

