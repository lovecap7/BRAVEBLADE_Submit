#pragma once
#include "GameCameraStateBase.h"
class GameCamera;
class ActorManager;
class GameCameraStateZMove :
	public GameCameraStateBase, public std::enable_shared_from_this<GameCameraStateZMove>
{
public:
	GameCameraStateZMove(std::weak_ptr<GameCamera> camera);
	virtual ~GameCameraStateZMove() {};
	//‰Šú‰»ˆ—
	void Init()override;
	//ó‘Ô‚É‰‚¶‚½XVˆ—
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
};

