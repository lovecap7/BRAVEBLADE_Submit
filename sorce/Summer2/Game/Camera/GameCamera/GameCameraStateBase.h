#pragma once
#include <memory>
#include "../../../General/Math/MyMath.h"
class GameCamera;
class ActorManager;
class GameCameraStateBase abstract
{
public:
	GameCameraStateBase(std::weak_ptr<GameCamera> camera);
	virtual ~GameCameraStateBase() {};
	//����������
	virtual void Init()abstract;
	//��Ԃɉ������X�V����
	virtual void Update(const std::weak_ptr<ActorManager> actorManager) abstract;
	//���̏�Ԃ��擾
	std::shared_ptr<GameCameraStateBase> GetNextState() { return m_nextState; };
	//��ԕω�
	virtual void ChangeState(std::shared_ptr<GameCameraStateBase> nextState);
private:
	//�J�����̏��
	std::shared_ptr<GameCameraStateBase> m_nextState;
protected:
	//�J�����̎�����
	std::weak_ptr<GameCamera> m_camera;
};

