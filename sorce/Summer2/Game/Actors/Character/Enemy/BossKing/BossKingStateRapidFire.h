#pragma once
#include "BossKingStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDatas.h"
#include "../../../../../General/Math/MyMath.h"
class GameCamera;
class Input;
class ActorManager;
class Actor;
class Breath;
class BossKingStateRapidFire :
	public BossKingStateBase, public std::enable_shared_from_this<BossKingStateRapidFire>
{
public:
	BossKingStateRapidFire(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager);
	~BossKingStateRapidFire();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
    //UŒ‚‚Ìì¬
    void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
    //UŒ‚ƒf[ƒ^
    AttackData m_attackData;
    //‹…‚ğ‘Å‚Á‚½”
    int m_shotNum;
};
