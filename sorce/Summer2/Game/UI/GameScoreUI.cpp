#include "GameScoreUI.h"
#include "../../General/game.h"
#include "../GameRule/Score.h"
namespace
{
	//�X�R�A��X���W
	constexpr int kPosX = (Game::kScreenCenterX + 200);
	//�X�R�A��Y���W
	constexpr int kPosY = 30;
	//�傫��
	constexpr float kScale = 0.1f;
	//�X�R�A��1���̕�
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
	//�X�R�A���폜���ꂽ�炱��UI���폜
	if (m_score.expired())
	{
		m_isDelete = true;
		return;
	}
	auto score = m_score.lock();
	//�X�R�A�X�V
	UpdateViewScore(score->GetKillScore() + score->GetItemScore());
}
