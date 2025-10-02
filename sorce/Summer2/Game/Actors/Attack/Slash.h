#pragma once
#include "CapsuleAttackBase.h"
class Slash :
    public CapsuleAttackBase
{
public:
    Slash(std::weak_ptr<Actor> owner);
    ~Slash();
    //XVˆ—
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    //•`‰æ
    void Draw()const override;
    //“–‚½‚Á‚½
    void OnCollide(const std::shared_ptr<Collidable> other)override;
};

