#include "TitleCamera.h"
#include <DxLib.h>

namespace
{
	//near��far
	constexpr float kNear = 50.0f;
	constexpr float kFar = 7000.0f;
	//�����ʒu
	const Vector3 kFirstPos = { -200,150,-700 };
	//�J�����̏����ʒu���猩�Ă���ʒu
	const Vector3 kViewPos = { -200,230,0 };
	//����p
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
	//���s50�`3000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(kNear, kFar);
	//�J�����̏����ʒu
	m_pos = kFirstPos;
	//�J���������Ă�ʒu
	m_viewPos = kViewPos;
	//�J�����̍��W�ƒ����_
	DxLib::SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
	//����p
	SetupCamera_Perspective(kPerspective);
}

void TitleCamera::Update()
{
}
