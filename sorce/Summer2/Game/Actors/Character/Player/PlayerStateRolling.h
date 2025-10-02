#pragma once
#include "PlayerStateBase.h"
class Actor;
class Input;
class GameCamera;
class ActorManager;
class Player;
class TrackActorEffect;
class PlayerStateRolling :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateRolling>
{
public:
    PlayerStateRolling(std::weak_ptr<Actor> player);
    ~PlayerStateRolling();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //ƒ‰ƒ“ƒ_ƒ€VC
    void RandRollingVC(std::shared_ptr<Player> coll);
};

