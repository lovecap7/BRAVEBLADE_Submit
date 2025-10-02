#pragma once
#include "SphereAttackBase.h"
//”ÍˆÍUŒ‚
class Actor;
class AreaOfEffectAttack :
    public SphereAttackBase
{
public:
    AreaOfEffectAttack(std::weak_ptr<Actor> owner);
    ~AreaOfEffectAttack();
    //XVˆ—
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    //•`‰æ
    void Draw()const override;
    void OnCollide(const std::shared_ptr<Collidable> other)override;
};

