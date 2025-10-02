#pragma once
#include "PurpleDinosaurStateBase.h"
class Input;
class GameCamera;
class ActorManager;
class Actor;
class Collidable;
class PurpleDinosaurStateChase :
    public PurpleDinosaurStateBase, public std::enable_shared_from_this<PurpleDinosaurStateChase>
{
public:
    PurpleDinosaurStateChase(std::weak_ptr<Actor> owner);
    ~PurpleDinosaurStateChase();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
  
};

