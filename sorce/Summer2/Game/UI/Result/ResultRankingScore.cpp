#include "ResultRankingScore.h"
#include "../UIManager.h"
#include <DxLib.h>
#include <format>

namespace
{
	//数字の座標オフセット
	constexpr int kNumOffsetX = 100;
	//ニューレコード
	constexpr int kNewRecordOffsetX = kNumOffsetX  - 10;
	constexpr int kNewRecordOffsetY = -50;
	constexpr int kNewRecordColor = 0xff2222; // 赤色
}

ResultRankingScore::ResultRankingScore(int scoreValue, Vector2 basePos, const std::wstring& text, int nowScore) :
	ResultScoreUI(scoreValue, basePos, text),
	m_rankingHandle(-1), // 初期化
	m_textColor(0xffffff)
{
	//もし現在のスコアとランキングの値が同じなら
	//ランキング入りをしているテキストを赤色に
	if(scoreValue == nowScore)
	{
		m_textColor = kNewRecordColor; // 赤色
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
	//テキスト
	DrawStringToHandle(m_basePos.x + kNumOffsetX, m_basePos.y, num.c_str(), m_textColor, m_textHandle);
	DrawRotaGraph(static_cast<int>(m_basePos.x), static_cast<int>(m_basePos.y),0.6,0.0, m_rankingHandle, true);
	//文字が赤色ならハイスコアを更新しているので
	if (m_textColor == kNewRecordColor)
	{
		//ハイスコアの更新を表示
		DrawStringToHandle(m_basePos.x + kNewRecordOffsetX, m_basePos.y + kNewRecordOffsetY, L"NewRecord!!!", 0xffff00, UIManager::GetInstance().GetTextHandle("源直ゴシック32"));
	}
}
