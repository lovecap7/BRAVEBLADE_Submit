#include "Fader.h"
#include "game.h"
#include "Input.h"
#include <DxLib.h>
//������
void Fader::Init()
{
	m_isFadeIn = false;
	m_isFadeOut = false;
	m_countFrame = 0.0f;
	m_fadeSpeed = 10.0f;
}

//�X�V
void Fader::Update()
{
	auto& input = Input::GetInstance();
	if (m_isFadeIn)
	{
		//���͂��~�߂�
		input.StopUpdate();
		//���񂾂񖾂邭�Ȃ�
		m_countFrame -= m_fadeSpeed;
		if (m_countFrame <= 0.0f)
		{
			//���͂��J�n
			input.StartUpdate();
			m_countFrame = 0.0f;
			m_isFadeIn = false;
		}
	}
	if (m_isFadeOut)
	{
		//���͂��~�߂�
		input.StopUpdate();
		//���񂾂�Â��Ȃ�
		m_countFrame += m_fadeSpeed;
		if (m_countFrame >= 255.0f)
		{
			//���͂��J�n
			input.StartUpdate();
			m_countFrame = 255.0f;
			m_isFadeOut = false;
		}
	}
	//�t�F�[�h���Ȃ�
	if (m_isFadeIn || m_isFadeOut)
	{
		m_isFadeNow = true;
	}
	else
	{
		m_isFadeNow = false;
	}
}
//�`��
void Fader::Draw()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(m_countFrame));
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
}

void Fader::FadeIn(float speed)
{
	if (!m_isFadeIn)
	{
		//���񂾂񖾂邭�Ȃ�
		m_countFrame = 255.0f;
		m_fadeSpeed = speed;
		m_isFadeIn = true;
		m_isFadeOut = false;
	}
}

void Fader::FadeOut(float speed)
{
	if (!m_isFadeOut)
	{
		//���񂾂�Â��Ȃ�
		m_countFrame = 0.0f;
		m_fadeSpeed = speed;
		m_isFadeOut = true;
		m_isFadeIn = false;
	}
}

bool Fader::IsFinishFadeIn()const
{
	return m_countFrame <= 0.0f && !m_isFadeOut;
}


bool Fader::IsFinishFadeOut() const
{
	return  m_countFrame >= 255.0f && !m_isFadeIn;
}