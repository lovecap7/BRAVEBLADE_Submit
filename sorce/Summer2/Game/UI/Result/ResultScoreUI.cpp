#include "ResultScoreUI.h"
#include "../../../General/game.h"
#include "../../GameRule/Score.h"
#include "../UIManager.h"
#include "../../../General/Input.h"
#include "../../../General/StringUtil.h"
#include <format>//0���ߗp

namespace
{
	//�����̍��W�I�t�Z�b�g
	constexpr int kNumOffsetX = 510;
}

ResultScoreUI::ResultScoreUI(int scoreValue, Vector2 basePos, const std::wstring& text):
	ScoreUIBase(basePos,-1,-1),
	m_scoreValue(scoreValue),
	m_countFrame(0),
	m_textHandle(UIManager::GetInstance().GetTextHandle("�����S�V�b�N48")),
	m_text(text)
{
}

ResultScoreUI::~ResultScoreUI()
{
}

void ResultScoreUI::Update()
{
	auto& input = Input::GetInstance();
	++m_countFrame;
	//�X�R�A���Z���I��������
	if (input.IsTrigger("A") && !IsViewScoreMax())
	{
		//A�{�^������������X�R�A���m��
		SetViewScore(m_scoreValue);
		return;
	}
	//�X�R�A�X�V
	UpdateViewScore(m_scoreValue);
}

void ResultScoreUI::Draw() const
{
	std::wstring num = L": " + std::format(L"{:06}", m_viewScore);
	//�e�L�X�g
	DrawStringToHandle(m_basePos.x, m_basePos.y, m_text.c_str(), 0x55ff00, m_textHandle);
	DrawStringToHandle(m_basePos.x + kNumOffsetX, m_basePos.y, num.c_str(), 0xffffff, m_textHandle);
}