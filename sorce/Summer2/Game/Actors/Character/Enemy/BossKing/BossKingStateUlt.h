#pragma once
#include "BossKingStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDataLoader.h"
#include "../../../../../General/Math/MyMath.h"
#include <list>

class GameCamera;
class Input;
class ActorManager;
class Actor;
class MyEffect;
class BossKingStateUlt :
	public BossKingStateBase, public std::enable_shared_from_this<BossKingStateUlt>
{
public:
	BossKingStateUlt(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager);
	~BossKingStateUlt();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
    //UŒ‚‚Ìì¬
    void CreateAttack(const std::weak_ptr<ActorManager> actorManager, Vector3 pos);
    //UŒ‚ƒf[ƒ^
    AttackData m_attackData;
    //‹…‚ğ‘Å‚Á‚½”
    int m_shotNum;
};

