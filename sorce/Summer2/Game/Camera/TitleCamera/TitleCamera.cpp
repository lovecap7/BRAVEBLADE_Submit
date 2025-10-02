#include "TitleCamera.h"
#include <DxLib.h>

namespace
{
	//nearとfar
	constexpr float kNear = 50.0f;
	constexpr float kFar = 7000.0f;
	//初期位置
	const Vector3 kFirstPos = { -200,150,-700 };
	//カメラの初期位置から見ている位置
	const Vector3 kViewPos = { -200,230,0 };
	//視野角
	constexpr float kPerspective = 35.0f * MyMath::DEG_2_RAD;
}

TitleCamera::TitleCamera():
	m_pos{},
	m_viewPos{}
{
}

TitleCamera::~TitleCamera()
{
}

void TitleCamera::Init()
{
	//奥行50～3000までをカメラの描画範囲とする
	SetCameraNearFar(kNear, kFar);
	//カメラの初期位置
	m_pos = kFirstPos;
	//カメラが見てる位置
	m_viewPos = kViewPos;
	//カメラの座標と注視点
	DxLib::SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
	//視野角
	SetupCamera_Perspective(kPerspective);
}

void TitleCamera::Update()
{
}
