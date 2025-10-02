#include "Score.h"
#include "../../General/Math/MathSub.h"
#include "../../General/HitPoints.h"
#include "../../General/CSVDataSaver.h"
#include "../../General/CSVDataLoader.h"

Score::Score()
{
	//スコアデータ
	m_csvLoader = std::make_unique<CSVDataLoader>();
	m_acotrScoreData = m_csvLoader->LoadActorScoreDataCSV();
	//ハイスコア
	m_highScore = m_csvLoader->LoadHighScoreDataCSV();
	//ランキングの並びチェック
	CheckRankingIntegrity();
}

Score::~Score()
{
}

void Score::Init()
{
	m_nowScore = 0;
	m_timeScore = 0;
	m_killScore = 0;
	m_itemScore = 0;
	m_hpScore = 0;
	m_hpScoreData = 0;
	m_timeScoreData = 0;
	m_decTimeScoreData = 0;
	//データに一致するものを探す
	for (auto data : m_acotrScoreData)
	{
		if (data.dataName == ScoreDataName::kHPScore)
		{
			m_hpScoreData = data.score;
		}
		else if (data.dataName == ScoreDataName::kTimeScore)
		{
			m_timeScoreData = data.score;
		}
		else if (data.dataName == ScoreDataName::kDecTimeScore)
		{
			m_decTimeScoreData = data.score;
		}
	}
}

void Score::UpdateScore(Stage::StageIndex index)
{
	//スコア更新
	m_nowScore = m_timeScore + m_killScore + m_itemScore + m_hpScore;
	//ハイスコアを更新
	UpdateRanking(index, GetScore());
}

int Score::GetScore()const
{
	return m_nowScore;
}

std::array<int, static_cast<int>(Stage::StageIndex::StageNum)> Score::GetHighScore(Stage::StageIndex index)const
{
	return m_highScore[static_cast<int>(index)];
}

void Score::AddTimeScore(int time)
{
	m_timeScore = m_timeScoreData - (m_decTimeScoreData * time);
	m_timeScore = MathSub::ClampInt(m_timeScore, 0, m_timeScoreData);
}

void Score::AddKillOrItemScore(std::string dataName)
{
		//データに一致するものを探す
		for (auto data : m_acotrScoreData)
		{
			//見つかった時
			if (dataName == data.dataName)
			{
				//敵なら
				if (dataName == ScoreDataName::kPurpleDinosaur	||
					dataName == ScoreDataName::kSmallDragon		||
					dataName == ScoreDataName::kBomber			||
					dataName == ScoreDataName::kBossDragon		||
					dataName == ScoreDataName::kBossMuscle)
				{
					m_killScore += data.score;
					break;
				}
				//アイテムなら
				else if (dataName == ScoreDataName::kAttackUp	||
						 dataName == ScoreDataName::kDefenseUp	||
						 dataName == ScoreDataName::kHeart		||
						 dataName == ScoreDataName::kUltGageUp)
				{
					m_itemScore += data.score;
					break;
				}
			}
		}
}

void Score::AddHPScore(std::weak_ptr<HitPoints> hp)
{
	auto hitPoint = hp.lock();
	float nowHP = static_cast<float>(hitPoint->GetHp());
	float maxHP = static_cast<float>(hitPoint->GetMaxHp());
	m_hpScore = m_hpScoreData * (nowHP / maxHP);
}

void Score::SaveHighScore()
{
	//保存する
	auto saver = std::make_shared<CSVDataSaver>();
	saver->SaveScoreDataToCSV(shared_from_this());
}
void Score::NewGame()
{
	for (int i = 0; i < m_highScore.size(); ++i)
	{
		m_highScore[i][0] = 0;
		m_highScore[i][1] = 0;
		m_highScore[i][2] = 0;
	}
	SaveHighScore();
}

void Score::CheckRankingIntegrity()
{
	for (int i = 0;i < m_highScore.size(); ++i)
	{
		//ランキングの順位通りの並びかチェック
		auto num1 = m_highScore[i][0];
		auto num2 = m_highScore[i][1];
		auto num3 = m_highScore[i][2];
		//1位よりも2位のほうが大きいなら
		if (num1 < num2)
		{
			auto temp = num1;
			num1 = num2;
			num2 = temp;
		}
		//1位よりも3位のほうが大きいなら
		if (num1 < num3)
		{
			auto temp = num1;
			num1 = num3;
			num3 = temp;
		}
		//2位よりも3位のほうが大きいなら
		if (num2 < num3)
		{
			auto temp = num2;
			num2 = num3;
			num3 = temp;
		}
		m_highScore[i][0] = num1;
		m_highScore[i][1] = num2;
		m_highScore[i][2] = num3;
	}
}

void Score::UpdateRanking(Stage::StageIndex index, int newScore)
{
	//ステージ
	auto stageIndex = static_cast<int>(index);
	for (int i = 2;i >= 0;--i)
	{
		//3位との比較でランキングが更新されなかったら終了
		if (i >= 2)
		{
			//3位との比較
			if (newScore > m_highScore[stageIndex][i])
			{
				//3位を更新
				m_highScore[stageIndex][i] = newScore;
				continue;
			}
			break;
		}
		//i位との比較
		if (newScore > m_highScore[stageIndex][i])
		{
			//i位を更新
			auto temp = m_highScore[stageIndex][i];
			m_highScore[stageIndex][i] = newScore;
			//i + 1位の値がi位に
			m_highScore[stageIndex][i + 1] = temp;
		}
	}
	//ランキングが正常か
	CheckRankingIntegrity();
}
