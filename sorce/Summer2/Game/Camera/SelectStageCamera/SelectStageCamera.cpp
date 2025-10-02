#include "SelectStageCamera.h"
#include <DxLib.h>

namespace
{
	//near��far
	constexpr float kNear = 50.0f;
	constexpr float kFar = 7000.0f;
	//�����ʒu
	const Vector3 kCameraOffsetPos = { 0,700,-1000 };
	//�J�����̏����ʒu���猩�Ă���ʒu
	const Vector3 kViewOffsetPos = { 0,300,0 };
	//����p
	constexpr float kPerspective = 35.0f * MyMath::DEG_2_RAD;
	//lerp��
	constexpr float kLerpRate = 0.1f;
}

SelectStageCamera::SelectStageCamera(Vector3 targetPos)
{
	//�����ʒu
	m_pos = targetPos;
	//�J�����̏����ʒu����
	m_pos += kCameraOffsetPos;
	//�J���������Ă�ʒu
	m_viewPos = targetPos;
}

SelectStageCamera::~SelectStageCamera()
{
}

void SelectStageCamera::Init()
{
	//���s50�`3000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(kNear, kFar);
	//�J�����̍��W�ƒ����_
	DxLib::SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
	//����p
	SetupCamera_Perspective(kPerspective);
}

void SelectStageCamera::Update(Vector3 targetPos)
{
	//���񂾂�ړI�n�Ɉړ�
	m_pos = Vector3::Lerp(m_pos, targetPos + kCameraOffsetPos, kLerpRate);
	m_viewPos = Vector3::Lerp(m_viewPos, targetPos + kViewOffsetPos, kLerpRate);
	//�J�����̍��W�ƒ����_
	DxLib::SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
}

