#pragma once
#include "BossKingStateBase.h"
class Input;
class GameCamera;
class ActorManager;
class Actor;
class BossKingStateIdle :
	public BossKingStateBase, public std::enable_shared_from_this<BossKingStateIdle>
{
public:
	BossKingStateIdle(std::weak_ptr<Actor> owner,bool isTransformSecond);
	~BossKingStateIdle();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:

};