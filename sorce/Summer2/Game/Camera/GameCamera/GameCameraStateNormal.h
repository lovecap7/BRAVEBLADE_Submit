#pragma once
#include "GameCameraStateBase.h"
class GameCameraStateNormal :
    public GameCameraStateBase, public std::enable_shared_from_this<GameCameraStateNormal>
{
public:
	GameCameraStateNormal(std::weak_ptr<GameCamera> camera);
	virtual ~GameCameraStateNormal() {};
	//����������
	void Init()override;
	//��Ԃɉ������X�V����
	void Update(const std::weak_ptr<ActorManager> actorManager) override;
private:
	//�J�����𒆐S(�^�[�Q�b�g)����X���C�h������
	float m_cameraSlide;
	//�X���C�h�J�n�t���[��
	int m_moveStartFrame;
	//�X���C�h�I���t���[��
	int m_moveEndFrame;
};

