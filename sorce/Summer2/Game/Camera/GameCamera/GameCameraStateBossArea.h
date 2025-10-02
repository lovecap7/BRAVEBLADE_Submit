#pragma once
#include "GameCameraStateBase.h"
class GameCameraStateBossArea :
	public GameCameraStateBase, public std::enable_shared_from_this<GameCameraStateBossArea>
{
public:
	GameCameraStateBossArea(std::weak_ptr<GameCamera> camera);
	virtual ~GameCameraStateBossArea() {};
	//����������
	void Init()override;
	//��Ԃɉ������X�V����
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
private:
	float m_angle;
};

