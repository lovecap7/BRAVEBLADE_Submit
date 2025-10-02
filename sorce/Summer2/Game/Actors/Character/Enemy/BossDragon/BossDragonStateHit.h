#pragma once
#include "BossDragonStateBase.h"
class Actor;
class GameCamera;
class ActorManager;
class BossDragonStateHit :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateHit>
{
public:
    BossDragonStateHit(std::weak_ptr<Actor> owner);
    ~BossDragonStateHit();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
};
