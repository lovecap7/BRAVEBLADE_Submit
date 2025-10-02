#pragma once
#include "BossMuscleStateBase.h"
class Actor;
class GameCamera;
class ActorManager;
class BossMuscleStateHit :
    public BossMuscleStateBase, public std::enable_shared_from_this<BossMuscleStateHit>
{
public:
    BossMuscleStateHit(std::weak_ptr<Actor> owner, bool isAngry);
    ~BossMuscleStateHit();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
};
