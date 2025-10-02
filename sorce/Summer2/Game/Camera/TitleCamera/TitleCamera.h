#pragma once
#include <memory>
#include "../../../General/Math/MyMath.h"

class TitleCamera
{
public:
	TitleCamera();
	~TitleCamera();
	//初期化処理
	void Init();
	//更新処理
	void Update();
private:
	//自分の座標
	Position3 m_pos;
	//カメラが見てる位置
	Vector3 m_viewPos;
};

