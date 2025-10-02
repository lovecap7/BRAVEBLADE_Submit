#pragma once
#include "../../../General/Math/MyMath.h"
class SelectStageCamera
{
public:
	SelectStageCamera(Vector3 targetPos);
	~SelectStageCamera();
	//������
	void Init();
	//�X�V
	void Update(Vector3 targetPos);
	//���W
	Position3 GetPos() const { return m_pos; }
private:
	//�����̍��W
	Position3 m_pos;
	//�J���������Ă�ʒu
	Vector3 m_viewPos;
};

