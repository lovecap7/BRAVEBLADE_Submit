#pragma once
#include "../UIBase.h"
#include "../../../General/Math/MyMath.h"
class ResultUI :
    public UIBase
{
public:
	ResultUI();
	~ResultUI();
	void Update()override;
	void Draw()const override;
private:
	//�n���h��
	int m_handle; 
	//�`��ʒu
	Vector2 m_pos;
};

