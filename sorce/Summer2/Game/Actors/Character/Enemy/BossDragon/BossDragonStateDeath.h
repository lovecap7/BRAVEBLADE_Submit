#pragma once
#include "BossDragonStateBase.h"
class Input;
class GameCamera;
class ActorManager;
class Actor;
class BossDragonStateDeath :
	public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateDeath>
{
public:
	BossDragonStateDeath(std::weak_ptr<Actor> owner);
	~BossDragonStateDeath();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:

};

