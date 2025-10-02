#pragma once
#include "BossDragonStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDataLoader.h"

class GameCamera;
class Actor;
class ActorManager;
class Collidable;
class Breath;
class BossDragonStateBreathAttack :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateBreathAttack>
{
public:
    BossDragonStateBreathAttack(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager);
    ~BossDragonStateBreathAttack();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
    //UŒ‚‚ÌQÆ
    std::weak_ptr<Breath> m_attack1;
    std::weak_ptr<Breath> m_attack2;
    std::weak_ptr<Breath> m_attack3;
    //UŒ‚‚Ìì¬
    void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
    //’e‚Ìó‘Ôİ’è
    void SetupBreath(std::weak_ptr<Breath> bullet, float angle);
    //UŒ‚ƒf[ƒ^
    AttackData m_attackData;
};

