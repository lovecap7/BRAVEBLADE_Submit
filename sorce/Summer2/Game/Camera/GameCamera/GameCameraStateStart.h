#pragma once
#include "GameCameraStateBase.h"
class GameCamera;
class ActorManager;
class GameCameraStateStart :
	public GameCameraStateBase, public std::enable_shared_from_this<GameCameraStateStart>
{
public:
	GameCameraStateStart(std::weak_ptr<GameCamera> camera);
	virtual ~GameCameraStateStart() {};
	//����������
	void Init()override;
	//��Ԃɉ������X�V����
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
private:
};