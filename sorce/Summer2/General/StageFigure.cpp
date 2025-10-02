#include "StageFigure.h"
#include "../General/Model.h"

namespace
{
	//���f���̈ʒu����
	const Vector3 kModelOffset = { 0.0f, 0.0f, 500.0f }; // ���f���̈ʒu�𒲐�����I�t�Z�b�g
	//��]
	constexpr float kRotationSpeed = 0.5f; // ��]���x
}

StageFigure::StageFigure()
{
}

StageFigure::~StageFigure()
{
}

void StageFigure::Init(int handle, Vector3 pos)
{
	auto newPos = pos + kModelOffset; // ���f���̈ʒu�𒲐�
	//���f���̏�����
	m_model = std::make_shared<Model>(handle, newPos.ToDxLibVector());
}

void StageFigure::Update()
{
	//�A�j���[�V�����̍X�V
	m_model->Update();
	//���f���̉�]
	m_model->SetRot(VGet(0.0f, kRotationSpeed, 0.0f));
}

void StageFigure::Draw() const
{
	//���f����`��
	m_model->Draw();
}


void StageFigure::End()
{
	//���f���̏I������
	m_model->End();
}
