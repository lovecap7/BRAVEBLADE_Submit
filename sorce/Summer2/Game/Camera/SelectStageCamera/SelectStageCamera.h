#pragma once
#include "../../../General/Math/MyMath.h"
class SelectStageCamera
{
public:
	SelectStageCamera(Vector3 targetPos);
	~SelectStageCamera();
	//初期化
	void Init();
	//更新
	void Update(Vector3 targetPos);
	//座標
	Position3 GetPos() const { return m_pos; }
private:
	//自分の座標
	Position3 m_pos;
	//カメラが見てる位置
	Vector3 m_viewPos;
};

