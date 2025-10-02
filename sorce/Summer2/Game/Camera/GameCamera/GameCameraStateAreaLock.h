#pragma once
#include "GameCameraStateBase.h"
class GameCameraStateAreaLock :
    public GameCameraStateBase, public std::enable_shared_from_this<GameCameraStateAreaLock>
{
public:
	GameCameraStateAreaLock(std::weak_ptr<GameCamera> camera);
	virtual ~GameCameraStateAreaLock() {};
	//����������
	void Init()override;
	//��Ԃɉ������X�V����
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
};

