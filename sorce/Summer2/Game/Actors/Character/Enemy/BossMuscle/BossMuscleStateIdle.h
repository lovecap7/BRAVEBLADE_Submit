#pragma once
#include "BossMuscleStateBase.h"
class Input;
class GameCamera;
class ActorManager;
class Actor;
class BossMuscleStateIdle :
	public BossMuscleStateBase, public std::enable_shared_from_this<BossMuscleStateIdle>
{
public:
	BossMuscleStateIdle(std::weak_ptr<Actor> owner, bool isAngry);
	~BossMuscleStateIdle();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	
};