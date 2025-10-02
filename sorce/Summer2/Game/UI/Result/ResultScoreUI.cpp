#include "ResultScoreUI.h"
#include "../../../General/game.h"
#include "../../GameRule/Score.h"
#include "../UIManager.h"
#include "../../../General/Input.h"
#include "../../../General/StringUtil.h"
#include <format>//0埋め用

namespace
{
	//数字の座標オフセット
	constexpr int kNumOffsetX = 510;
}

ResultScoreUI::ResultScoreUI(int scoreValue, Vector2 basePos, const std::wstring& text):
	ScoreUIBase(basePos,-1,-1),
	m_scoreValue(scoreValue),
	m_countFrame(0),
	m_textHandle(UIManager::GetInstance().GetTextHandle("源直ゴシック48")),
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
	//スコア加算を終了させる
	if (input.IsTrigger("A") && !IsViewScoreMax())
	{
		//Aボタンを押したらスコアを確定
		SetViewScore(m_scoreValue);
		return;
	}
	//スコア更新
	UpdateViewScore(m_scoreValue);
}

void ResultScoreUI::Draw() const
{
	std::wstring num = L": " + std::format(L"{:06}", m_viewScore);
	//テキスト
	DrawStringToHandle(m_basePos.x, m_basePos.y, m_text.c_str(), 0x55ff00, m_textHandle);
	DrawStringToHandle(m_basePos.x + kNumOffsetX, m_basePos.y, num.c_str(), 0xffffff, m_textHandle);
}