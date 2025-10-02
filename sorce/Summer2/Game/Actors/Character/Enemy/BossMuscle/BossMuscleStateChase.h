#pragma once
#include "BossMuscleStateBase.h"
class BossMuscleStateChase :
	public BossMuscleStateBase, public std::enable_shared_from_this<BossMuscleStateChase>
{
public:
	BossMuscleStateChase(std::weak_ptr<Actor> owner, bool isAngry);
	~BossMuscleStateChase();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
};

