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
	//����������
	void Init()override;
	//��Ԃɉ������X�V����
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
};

