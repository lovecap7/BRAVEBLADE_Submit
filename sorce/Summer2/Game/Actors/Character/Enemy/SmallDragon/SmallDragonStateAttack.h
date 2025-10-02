#pragma once
#include "SmallDragonStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDataLoader.h"

class GameCamera;
class Actor;
class ActorManager;
class Collidable;
class Bullet;
class SmallDragonStateAttack :
	public SmallDragonStateBase, public std::enable_shared_from_this<SmallDragonStateAttack>
{
public:
	SmallDragonStateAttack(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager);
	~SmallDragonStateAttack();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
	int m_attackCountFrame;
	//UŒ‚‚ÌQÆ
	std::weak_ptr<Bullet> m_attack;
	//UŒ‚‚Ìì¬
	void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
	//UŒ‚ƒf[ƒ^
	AttackData m_attackData;
};

