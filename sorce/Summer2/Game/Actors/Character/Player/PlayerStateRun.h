#pragma once
#include "PlayerStateBase.h""
class Player;
class Input;
class GameCamera;
class ActorManager;
class PlayerStateRun :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateRun>
{
public:
    PlayerStateRun(std::weak_ptr<Actor>  player);
    ~PlayerStateRun();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
  
};

