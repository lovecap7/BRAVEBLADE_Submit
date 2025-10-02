#pragma once
#include "GameCameraStateBase.h"
class ActorManager;
class GameCameraStateClear :
	public GameCameraStateBase, public std::enable_shared_from_this<GameCameraStateClear>
{
public:
	GameCameraStateClear(std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager);
	virtual ~GameCameraStateClear() {};
	//����������
	void Init()override;
	//��Ԃɉ������X�V����
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
private:
	int m_rotaFrame;
};

