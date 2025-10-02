#pragma once
#include "GameCameraStateBase.h"
class GameCameraStateAreaLock :
    public GameCameraStateBase, public std::enable_shared_from_this<GameCameraStateAreaLock>
{
public:
	GameCameraStateAreaLock(std::weak_ptr<GameCamera> camera);
	virtual ~GameCameraStateAreaLock() {};
	//‰Šú‰»ˆ—
	void Init()override;
	//ó‘Ô‚É‰‚¶‚½XVˆ—
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
};

