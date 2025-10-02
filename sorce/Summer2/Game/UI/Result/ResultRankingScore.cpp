#include "ResultRankingScore.h"
#include "../UIManager.h"
#include <DxLib.h>
#include <format>

namespace
{
	//�����̍��W�I�t�Z�b�g
	constexpr int kNumOffsetX = 100;
	//�j���[���R�[�h
	constexpr int kNewRecordOffsetX = kNumOffsetX  - 10;
	constexpr int kNewRecordOffsetY = -50;
	constexpr int kNewRecordColor = 0xff2222; // �ԐF
}

ResultRankingScore::ResultRankingScore(int scoreValue, Vector2 basePos, const std::wstring& text, int nowScore) :
	ResultScoreUI(scoreValue, basePos, text),
	m_rankingHandle(-1), // ������
	m_textColor(0xffffff)
{
	//�������݂̃X�R�A�ƃ����L���O�̒l�������Ȃ�
	//�����L���O��������Ă���e�L�X�g��ԐF��
	if(scoreValue == nowScore)
	{
		m_textColor = kNewRecordColor; // �ԐF
	}
	auto& uiManager = UIManager::GetInstance();
	if(text == L"1st")
	{
		m_rankingHandle = uiManager.GetImageHandle("1st");
	}
	else if (text == L"2nd")
	{
		m_rankingHandle = uiManager.GetImageHandle("2nd");
	}
	else if (text == L"3rd")
	{
		m_rankingHandle = uiManager.GetImageHandle("3rd");
	}
}

ResultRankingScore::~ResultRankingScore()
{
}

void ResultRankingScore::Draw() const
{
	std::wstring num = L": " + std::format(L"{:06}", m_viewScore);
	//�e�L�X�g
	DrawStringToHandle(m_basePos.x + kNumOffsetX, m_basePos.y, num.c_str(), m_textColor, m_textHandle);
	DrawRotaGraph(static_cast<int>(m_basePos.x), static_cast<int>(m_basePos.y),0.6,0.0, m_rankingHandle, true);
	//�������ԐF�Ȃ�n�C�X�R�A���X�V���Ă���̂�
	if (m_textColor == kNewRecordColor)
	{
		//�n�C�X�R�A�̍X�V��\��
		DrawStringToHandle(m_basePos.x + kNewRecordOffsetX, m_basePos.y + kNewRecordOffsetY, L"NewRecord!!!", 0xffff00, UIManager::GetInstance().GetTextHandle("�����S�V�b�N32"));
	}
}
