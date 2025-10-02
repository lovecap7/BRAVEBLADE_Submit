#pragma once
#include "BossDragonStateBase.h"
class Input;
class GameCamera;
class ActorManager;
class Actor;
class BossDragonStateChase :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateChase>
{
public:
    BossDragonStateChase(std::weak_ptr<Actor> owner);
    ~BossDragonStateChase();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:

};

