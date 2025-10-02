#include "SelectStageCamera.h"
#include <DxLib.h>

namespace
{
	//nearとfar
	constexpr float kNear = 50.0f;
	constexpr float kFar = 7000.0f;
	//初期位置
	const Vector3 kCameraOffsetPos = { 0,700,-1000 };
	//カメラの初期位置から見ている位置
	const Vector3 kViewOffsetPos = { 0,300,0 };
	//視野角
	constexpr float kPerspective = 35.0f * MyMath::DEG_2_RAD;
	//lerp率
	constexpr float kLerpRate = 0.1f;
}

SelectStageCamera::SelectStageCamera(Vector3 targetPos)
{
	//初期位置
	m_pos = targetPos;
	//カメラの初期位置調整
	m_pos += kCameraOffsetPos;
	//カメラが見てる位置
	m_viewPos = targetPos;
}

SelectStageCamera::~SelectStageCamera()
{
}

void SelectStageCamera::Init()
{
	//奥行50～3000までをカメラの描画範囲とする
	SetCameraNearFar(kNear, kFar);
	//カメラの座標と注視点
	DxLib::SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
	//視野角
	SetupCamera_Perspective(kPerspective);
}

void SelectStageCamera::Update(Vector3 targetPos)
{
	//だんだん目的地に移動
	m_pos = Vector3::Lerp(m_pos, targetPos + kCameraOffsetPos, kLerpRate);
	m_viewPos = Vector3::Lerp(m_viewPos, targetPos + kViewOffsetPos, kLerpRate);
	//カメラの座標と注視点
	DxLib::SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
}

