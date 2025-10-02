#pragma once
#include "SmallDragonStateBase.h"
class Input;
class GameCamera;
class ActorManager;
class Actor;
class SmallDragonStateBack :
	public SmallDragonStateBase, public std::enable_shared_from_this<SmallDragonStateBack>
{
public:
	SmallDragonStateBack(std::weak_ptr<Actor> owner);
	~SmallDragonStateBack();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:

};





