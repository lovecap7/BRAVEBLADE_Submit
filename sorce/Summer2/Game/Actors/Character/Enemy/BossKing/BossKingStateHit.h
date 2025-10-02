#pragma once
#include "BossKingStateBase.h"

class Actor;
class GameCamera;
class ActorManager;
class BossKingStateHit :
    public BossKingStateBase, public std::enable_shared_from_this<BossKingStateHit>
{
public:
    BossKingStateHit(std::weak_ptr<Actor> owner, bool isTransformSecond);
    ~BossKingStateHit();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
};
