#pragma once
#include "SmallDragonStateBase.h"
class Actor;
class Input;
class GameCamera;
class ActorManager;
class SmallDragonStateHit :
    public SmallDragonStateBase, public std::enable_shared_from_this<SmallDragonStateHit>
{
public:
    SmallDragonStateHit(std::weak_ptr<Actor> owner);
    ~SmallDragonStateHit();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
};
