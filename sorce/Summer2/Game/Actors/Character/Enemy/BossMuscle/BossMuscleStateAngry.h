#pragma once
#include "BossMuscleStateBase.h"
class Input;
class GameCamera;
class ActorManager;
class Actor;
class BossMuscleStateAngry :
    public BossMuscleStateBase, public std::enable_shared_from_this<BossMuscleStateAngry>
{
public:
	BossMuscleStateAngry(std::weak_ptr<Actor> owner);
	~BossMuscleStateAngry();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
};

