#pragma once
#include "BossDragonStateBase.h"
class Input;
class GameCamera;
class ActorManager;
class Actor;
class BossDragonStateIdle :
	public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateIdle>
{
public:
	BossDragonStateIdle(std::weak_ptr<Actor> owner);
	~BossDragonStateIdle();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:

};