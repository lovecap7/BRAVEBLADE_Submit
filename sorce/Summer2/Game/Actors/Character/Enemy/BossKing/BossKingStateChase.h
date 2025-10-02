#pragma once
#include "BossKingStateBase.h"
class Input;
class GameCamera;
class ActorManager;
class Actor;
class BossKingStateChase :
    public BossKingStateBase, public std::enable_shared_from_this<BossKingStateChase>
{
public:
    BossKingStateChase(std::weak_ptr<Actor> owner, bool isTransformSecond);
    ~BossKingStateChase();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:

};

