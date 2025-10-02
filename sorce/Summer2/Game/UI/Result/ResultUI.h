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
	//ƒnƒ“ƒhƒ‹
	int m_handle; 
	//•`‰æˆÊ’u
	Vector2 m_pos;
};

