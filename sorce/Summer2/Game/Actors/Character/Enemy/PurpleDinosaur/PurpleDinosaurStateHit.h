#pragma once
#include "PurpleDinosaurStateBase.h"
class Actor;
class Input;
class GameCamera;
class ActorManager;
class PurpleDinosaurStateHit :
    public PurpleDinosaurStateBase, public std::enable_shared_from_this<PurpleDinosaurStateHit>
{
public:
    PurpleDinosaurStateHit(std::weak_ptr<Actor> owner);
    ~PurpleDinosaurStateHit();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
};
