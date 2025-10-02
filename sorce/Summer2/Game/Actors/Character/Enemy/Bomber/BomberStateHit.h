#pragma once
#include "BomberStateBase.h"
class Actor;
class Input;
class GameCamera;
class ActorManager;
class BomberStateHit :
    public BomberStateBase, public std::enable_shared_from_this<BomberStateHit>
{
public:
    BomberStateHit(std::weak_ptr<Actor> owner);
    ~BomberStateHit();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
};
