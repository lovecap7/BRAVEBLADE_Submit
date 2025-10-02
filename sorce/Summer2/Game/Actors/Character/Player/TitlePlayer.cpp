#include "TitlePlayer.h"
#include <DxLib.h>
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"

namespace
{
	//�ҋ@�A�j��
	const char* kIdleAnim = "Anim|Idle";
	//����
	const char* kWalkAnim = "Anim|Walk";
	//�A�j���[�V�������x
	constexpr float kWalkAnimSpeed = 0.5f;
}

TitlePlayer::TitlePlayer():
	CharacterBase(Shape::None)
{
	//���f��
	m_model = std::make_shared<Model>(MV1LoadModel(L"Data/Model/Player/Title/TitlePlayer.mv1"), Vector3::Zero().ToDxLibVector());
}

TitlePlayer::~TitlePlayer()
{
}

void TitlePlayer::Init()
{
	//�A�j���[�V����
	m_model->SetAnim(kIdleAnim, true);
}

void TitlePlayer::Update(bool isDecide)
{
	if (isDecide)
	{
		//�A�j���[�V����������ɕύX
		m_model->SetAnim(kWalkAnim, false, kWalkAnimSpeed);
	}
	m_model->Update();
}

void TitlePlayer::Draw() const
{
	//���f����`��
	m_model->Draw();
}

void TitlePlayer::End()
{
	//���f���̏I������
	m_model->End();
}

void TitlePlayer::InitSound()
{
}


