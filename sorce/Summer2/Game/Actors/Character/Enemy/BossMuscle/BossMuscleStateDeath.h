#pragma once
#include "BossMuscleStateBase.h"
class Input;
class GameCamera;
class ActorManager;
class Actor;
class BossMuscleStateDeath :
	public BossMuscleStateBase, public std::enable_shared_from_this<BossMuscleStateDeath>
{
public:
	BossMuscleStateDeath(std::weak_ptr<Actor> owner);
	~BossMuscleStateDeath();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:

};

