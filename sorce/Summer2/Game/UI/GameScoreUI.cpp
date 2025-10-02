#include "GameScoreUI.h"
#include "../../General/game.h"
#include "../GameRule/Score.h"
namespace
{
	//スコアのX座標
	constexpr int kPosX = (Game::kScreenCenterX + 200);
	//スコアのY座標
	constexpr int kPosY = 30;
	//大きさ
	constexpr float kScale = 0.1f;
	//スコアの1桁の幅
	constexpr int kDigitMargin = 13;
}

GameScoreUI::GameScoreUI(const std::weak_ptr<Score> score):
	ScoreUIBase(Vector2{ kPosX ,kPosY},kScale,kDigitMargin),
	m_score(score)
{
}

GameScoreUI::~GameScoreUI()
{
}

void GameScoreUI::Update()
{
	//スコアが削除されたらこのUIも削除
	if (m_score.expired())
	{
		m_isDelete = true;
		return;
	}
	auto score = m_score.lock();
	//スコア更新
	UpdateViewScore(score->GetKillScore() + score->GetItemScore());
}
