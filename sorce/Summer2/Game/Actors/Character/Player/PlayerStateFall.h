#pragma once
#include "PlayerStateBase.h"
class Actor;
class Input;
class GameCamera;
class ActorManager;
class PlayerStateFall :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateFall>
{
public:
    PlayerStateFall(std::weak_ptr<Actor>  player);
    ~PlayerStateFall();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //入力でスピードを返す
    float InputValueSpeed(const Input& input);
};

