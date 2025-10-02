#pragma once
#include "BossDragonStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDataLoader.h"
class GameCamera;
class Actor;
class ActorManager;
class Collidable;
class Strike;
class BossDragonStateSweepAttack :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateSweepAttack>
{
public:
    BossDragonStateSweepAttack(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager);
    ~BossDragonStateSweepAttack();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
    //‰ñ“]—Ê
    float m_attackRotaAngle;
    //UŒ‚‚ÌQÆ
    std::weak_ptr<Strike> m_attack;
    //UŒ‚‚Ìì¬
    void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
    //UŒ‚‚ÌˆÊ’u‚ğXV‚·‚é
    void UpdateAttackPos();
    //UŒ‚ƒf[ƒ^
    AttackData m_attackData;
};


