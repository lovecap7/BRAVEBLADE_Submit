#pragma once
#include "BossKingStateBase.h"
class BossKingStateDeath :
	public BossKingStateBase, public std::enable_shared_from_this<BossKingStateDeath>
{
public:
	BossKingStateDeath(std::weak_ptr<Actor> owner);
	~BossKingStateDeath();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:

};

