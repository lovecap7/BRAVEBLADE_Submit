#pragma once
#include "GameCameraStateBase.h"
class GameCamera;
class ActorManager;
class GameCameraStateBossStart :
	public GameCameraStateBase, public std::enable_shared_from_this<GameCameraStateBossStart>
{
public:
	GameCameraStateBossStart(std::weak_ptr<GameCamera> camera);
	virtual ~GameCameraStateBossStart() {};
	//����������
	void Init()override;
	//��Ԃɉ������X�V����
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
private:
};