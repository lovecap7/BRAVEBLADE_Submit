#pragma once
#include <memory>
#include "../../../General/Math/MyMath.h"

class TitleCamera
{
public:
	TitleCamera();
	~TitleCamera();
	//����������
	void Init();
	//�X�V����
	void Update();
private:
	//�����̍��W
	Position3 m_pos;
	//�J���������Ă�ʒu
	Vector3 m_viewPos;
};

