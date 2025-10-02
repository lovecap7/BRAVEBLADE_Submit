#pragma once
#include "BossKingStateBase.h"
class BossKingStateChange :
	public BossKingStateBase, public std::enable_shared_from_this<BossKingStateChange>
{
public:
	BossKingStateChange(std::weak_ptr<Actor> owner);
	~BossKingStateChange();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:

};

