#include "SelectStagePlayer.h"
#include <DxLib.h>
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Rigidbody.h"

namespace
{
	//�_���X
	const char* kDance1Anim = "Select|Dance1";
	const char* kDance2Anim = "Select|Dance2";
	const char* kDance3Anim = "Select|Dance3";
	//�_���X�̐�
	constexpr int kDanceNum = 3;
	//����
	const char* kRunAnim = "Select|Run";
	//����
	const char* kDecideAnim = "Select|Decide";
	//lerp��
	constexpr float kLerpRate = 0.05f;
	//�ړI�n�ɓ��B�����Ƃ݂Ȃ��͈�
	constexpr float kTargetReachRadius = 100.0f;
}

SelectStagePlayer::SelectStagePlayer(Vector3 cameraPos, Vector3 pos):
	CharacterBase(Shape::None),
	m_isChangeDance(false)
{
	//���f��
	m_model = std::make_shared<Model>(MV1LoadModel(L"Data/Model/Player/SelectStage/SelectStagePlayer.mv1"), Vector3::Zero().ToDxLibVector());
	//�����ʒu
	m_rb->m_pos = pos;
	m_model->SetPos(pos.ToDxLibVector());
	//�J������������������
	Vector3 dir = (cameraPos - m_rb->m_pos);
	m_model->SetDir(dir.XZ());
}

SelectStagePlayer::~SelectStagePlayer()
{
}

void SelectStagePlayer::Init()
{
	//�A�j���[�V����
	m_model->SetAnim(kDance3Anim, true);
	//�t���O��true��
	m_isChangeDance = true;
}

void SelectStagePlayer::Update(Vector3 cameraPos, Vector3 targetPos)
{
	auto& input = Input::GetInstance();
	if (input.IsTrigger("A") && !m_isDecided)
	{
		//�J������������������
		Vector3 dir = (cameraPos - m_rb->m_pos);
		m_model->SetDir(dir.XZ());
		//����
		m_model->SetAnim(kDecideAnim, false);
		m_isDecided = true;
	}
	//�A�j���[�V�����̍X�V
	m_model->Update();
	//���񂾂�ړI�n�Ɉړ�
	m_rb->m_pos = Vector3::Lerp(m_rb->m_pos, targetPos, kLerpRate);
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
	if (!m_isDecided)
	{
		if ((m_rb->m_pos - targetPos).Magnitude() > kTargetReachRadius)
		{
			//�i�s�����Ƀ��f������������
			Vector3 dir = (targetPos - m_rb->m_pos);
			m_model->SetDir(dir.XZ());
			//�A�j���[�V����
			m_model->SetAnim(kRunAnim, true);
			m_isChangeDance = false;
		}
		else if (!m_isChangeDance)
		{
			//�J������������������
			Vector3 dir = (cameraPos - m_rb->m_pos);
			m_model->SetDir(dir.XZ());
			//�����_���ɗx������߂�
			switch (GetRand(kDanceNum) % kDanceNum + 1)
			{
			case 1:
				m_model->SetAnim(kDance1Anim, true);
				break;
			case 2:
				m_model->SetAnim(kDance2Anim, true);
				break;
			case 3:
				m_model->SetAnim(kDance3Anim, true);
				break;
			}
			m_isChangeDance = true;
		}
	}
}

void SelectStagePlayer::Draw() const
{
	//���f����`��
	m_model->Draw();
}

void SelectStagePlayer::End()
{
	//���f���̏I������
	m_model->End();
}

void SelectStagePlayer::InitSound()
{
}
