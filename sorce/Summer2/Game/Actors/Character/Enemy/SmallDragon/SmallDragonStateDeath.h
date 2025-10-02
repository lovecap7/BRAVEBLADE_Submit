#pragma once
#include "SmallDragonStateBase.h"
class Input;
class GameCamera;
class ActorManager;
class Actor;
class SmallDragonStateDeath :
	public SmallDragonStateBase, public std::enable_shared_from_this<SmallDragonStateDeath>
{
public:
	SmallDragonStateDeath(std::weak_ptr<Actor> owner);
	~SmallDragonStateDeath();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:

};



