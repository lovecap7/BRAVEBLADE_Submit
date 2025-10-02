#pragma once
#include "PlayerStateBase.h"
class Actor;
class Input;
class GameCamera;
class ActorManager;
class PlayerStateWalk :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateWalk>
{
public:
    PlayerStateWalk(std::weak_ptr<Actor>  player);
    ~PlayerStateWalk();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //“ü—Í‚Ì‘å‚«‚³‚É‡‚í‚¹‚Ä‘¬“x‚ğ•Ô‚·
    float InputValueSpeed(const Input& input);
};

