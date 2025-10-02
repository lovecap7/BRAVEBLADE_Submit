#include "ScoreUIBase.h"
#include "../../General/game.h"
#include "../GameRule/Score.h"
#include "../UI/UIManager.h"
#include <DxLib.h>

namespace
{
	//���Z�̊�{�l
	constexpr int kAddBaseSpeed = 128;
	//���̊���
	constexpr float kAddDiffRate = 0.1f;
	//��������
	constexpr int kAddLowSpeedRate = 6;
	//�������鍷
	constexpr int kChangeLowSpeed = 300;
	//��
	constexpr int kImageWidth = 256;
	constexpr int kImageHeight = 256;
	//�d��
	constexpr float kGravity = 1.0f;
	//�W�����v��
	constexpr float kJumpPower = 2.0f;
}

ScoreUIBase::ScoreUIBase(Vector2 basePos, float scale, float digitMargin):
	UIBase(),
	m_basePos(basePos),
	m_scale(scale),
	m_digitMargin(digitMargin),
	m_viewScore(0),
	m_viewMaxScore(0),
	m_digits{},
	m_viewVecs{},
	m_handle(UIManager::GetInstance().GetImageHandle("Score"))
{
	for (auto& pos : m_viewPoses)
	{
		pos = m_basePos;
	}
}

ScoreUIBase::~ScoreUIBase()
{
}

void ScoreUIBase::Draw() const
{
	if (!m_isDraw)return;
	for (int i = 0;i < kDigitNum;++i)
	{
		//�؂�����v�Z����
		int sizeX, sizeY;
		GetGraphSize(m_handle, &sizeX, &sizeY);//�摜�T�C�Y
		int cutX = m_digits[i] % (sizeX / kImageWidth);//��
		int cutY = m_digits[i] / (sizeX / kImageWidth);//�c
		//�`��
		DrawRectRotaGraphFast(m_viewPoses[i].x - i * m_digitMargin, m_viewPoses[i].y,
			kImageWidth * cutX,
			kImageHeight * cutY,
			kImageWidth, kImageHeight,
			m_scale, 0.0f, m_handle, true, false);
	}
}

void ScoreUIBase::UpdateViewScore(int viewScore)
{
	m_viewMaxScore = viewScore;
	if (m_viewMaxScore <= 0)
	{
		m_viewScore = 0;
	}
	else
	{
		//���݂̃X�R�A�ƖڕW�X�R�A�̍�
		int diff = m_viewMaxScore - m_viewScore;
		//��������Ȃ�
		if (diff != 0)
		{
			int absDiff = abs(diff);
			//���Z����X�R�A�̊�{�l
			int speed = kAddBaseSpeed + (absDiff * kAddDiffRate);
			//�ڕW�ɋ߂��Ȃ班������
			if (absDiff < kChangeLowSpeed)
			{
				//�덷���߂�+1
				speed = absDiff / kAddLowSpeedRate + 1;
			}
			if (absDiff <= speed) {
				//�قڍ����Ȃ��̂ň�C�ɖڕW�l��
				m_viewScore = m_viewMaxScore;
			}
			else
			{
				// �X�R�A��step����������
				m_viewScore += speed;
			}
		}
	}
	//���˂�
	BounceScore(kJumpPower, m_basePos.y);
}

void ScoreUIBase::BounceScore(int jumpPower, int groundY)
{
	//���o���l
	int scoreValue = m_viewScore;
	for (int i = 0;i < kDigitNum;++i)
	{
		//���݂̒l
		auto digit = m_digits[i];
		//���o���ĕۑ�
		m_digits[i] = scoreValue % 10;
		//����������
		scoreValue = scoreValue / 10;
		//�����ς���Ă�Ȃ�
		if (digit != m_digits[i])
		{
			//�����͂˂�
			m_viewVecs[i].y -= jumpPower;
		}
		//����ȏ�Y���W��������Ȃ��悤�ɕ␳
		m_viewVecs[i].y += kGravity;
		if (m_viewPoses[i].y > groundY)
		{
			m_viewPoses[i].y = groundY;
		}
	}
}