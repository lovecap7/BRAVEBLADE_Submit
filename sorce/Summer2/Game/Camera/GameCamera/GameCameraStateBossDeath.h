#pragma once
#include "GameCameraStateBase.h"
class GameCameraStateBossDeath :
	public GameCameraStateBase, public std::enable_shared_from_this<GameCameraStateBossDeath>
{
public:
	GameCameraStateBossDeath(std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager);
	virtual ~GameCameraStateBossDeath() {};
	//����������
	void Init()override;
	//��Ԃɉ������X�V����
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
};

