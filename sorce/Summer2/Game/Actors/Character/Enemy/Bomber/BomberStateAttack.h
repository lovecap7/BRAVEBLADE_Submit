#pragma once
#include "BomberStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDataLoader.h"
class GameCamera;
class Actor;
class ActorManager;
class BomberStateAttack :
    public BomberStateBase, public std::enable_shared_from_this<BomberStateAttack>
{
public:
    BomberStateAttack(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager);
    ~BomberStateAttack();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    
private:
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
    //”š’e‚Ì¶¬
    void CreateBomb(const std::weak_ptr<ActorManager> actorManager);
    //UŒ‚ƒf[ƒ^
    AttackData m_attackData;
};