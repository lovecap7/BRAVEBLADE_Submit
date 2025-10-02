#pragma once
#include "GameCameraStateBase.h"
class ActorManager;
class GameCameraStateClear :
	public GameCameraStateBase, public std::enable_shared_from_this<GameCameraStateClear>
{
public:
	GameCameraStateClear(std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager);
	virtual ~GameCameraStateClear() {};
	//‰Šú‰»ˆ—
	void Init()override;
	//ó‘Ô‚É‰‚¶‚½XVˆ—
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
private:
	int m_rotaFrame;
};

