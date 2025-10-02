#pragma once
#include "PlayerStateBase.h"
class Actor;
class Input;
class GameCamera;
class ActorManager;
class PlayerStateDeath :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateDeath>
{
public:
    PlayerStateDeath(std::weak_ptr<Actor>  player);
    ~PlayerStateDeath();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    
};

