#pragma once
#include "PlayerUIBase.h"
#include <memory>
class Player;
class PlayerUltGageUI :
	public PlayerUIBase
{
public:
	PlayerUltGageUI(std::weak_ptr<Player> player);
	~PlayerUltGageUI();
	void Update()override;
	void Draw()const override;
private:
	//�Q�[�W
	float m_viewUltGageValue;
	float m_viewMaxUltGageValue;	
	//�Q�[�W�̃t���[���n���h��
	int m_ultGageHandle;
	int m_maxUltGageHandle;
	//�Q�[�W�̉摜�n���h��
	int m_ultGageFrameHandle;
	//�Q�[�W�����^���̎��̃{�^���̉摜�n���h��
	int m_ultBottunHandle;
	//�{�^���̊g��k��
	double m_bottunScale;
	//�g��k���Ɏg���p�x
	float m_bottunAngle;
};
