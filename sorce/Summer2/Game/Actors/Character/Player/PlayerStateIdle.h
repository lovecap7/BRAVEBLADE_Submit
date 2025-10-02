#pragma once
#include "PlayerStateBase.h"
class Actor;
class Input;
class GameCamera;
class ActorManager;
class PlayerStateIdle :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateIdle>
{
public:
    PlayerStateIdle(std::weak_ptr<Actor>  player);
    ~PlayerStateIdle();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
};

