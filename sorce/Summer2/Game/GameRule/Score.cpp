#include "Score.h"
#include "../../General/Math/MathSub.h"
#include "../../General/HitPoints.h"
#include "../../General/CSVDataSaver.h"
#include "../../General/CSVDataLoader.h"

Score::Score()
{
	//�X�R�A�f�[�^
	m_csvLoader = std::make_unique<CSVDataLoader>();
	m_acotrScoreData = m_csvLoader->LoadActorScoreDataCSV();
	//�n�C�X�R�A
	m_highScore = m_csvLoader->LoadHighScoreDataCSV();
	//�����L���O�̕��у`�F�b�N
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
	//�f�[�^�Ɉ�v������̂�T��
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
	//�X�R�A�X�V
	m_nowScore = m_timeScore + m_killScore + m_itemScore + m_hpScore;
	//�n�C�X�R�A���X�V
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
		//�f�[�^�Ɉ�v������̂�T��
		for (auto data : m_acotrScoreData)
		{
			//����������
			if (dataName == data.dataName)
			{
				//�G�Ȃ�
				if (dataName == ScoreDataName::kPurpleDinosaur	||
					dataName == ScoreDataName::kSmallDragon		||
					dataName == ScoreDataName::kBomber			||
					dataName == ScoreDataName::kBossDragon		||
					dataName == ScoreDataName::kBossMuscle)
				{
					m_killScore += data.score;
					break;
				}
				//�A�C�e���Ȃ�
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
	//�ۑ�����
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
		//�����L���O�̏��ʒʂ�̕��т��`�F�b�N
		auto num1 = m_highScore[i][0];
		auto num2 = m_highScore[i][1];
		auto num3 = m_highScore[i][2];
		//1�ʂ���2�ʂ̂ق����傫���Ȃ�
		if (num1 < num2)
		{
			auto temp = num1;
			num1 = num2;
			num2 = temp;
		}
		//1�ʂ���3�ʂ̂ق����傫���Ȃ�
		if (num1 < num3)
		{
			auto temp = num1;
			num1 = num3;
			num3 = temp;
		}
		//2�ʂ���3�ʂ̂ق����傫���Ȃ�
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
	//�X�e�[�W
	auto stageIndex = static_cast<int>(index);
	for (int i = 2;i >= 0;--i)
	{
		//3�ʂƂ̔�r�Ń����L���O���X�V����Ȃ�������I��
		if (i >= 2)
		{
			//3�ʂƂ̔�r
			if (newScore > m_highScore[stageIndex][i])
			{
				//3�ʂ��X�V
				m_highScore[stageIndex][i] = newScore;
				continue;
			}
			break;
		}
		//i�ʂƂ̔�r
		if (newScore > m_highScore[stageIndex][i])
		{
			//i�ʂ��X�V
			auto temp = m_highScore[stageIndex][i];
			m_highScore[stageIndex][i] = newScore;
			//i + 1�ʂ̒l��i�ʂ�
			m_highScore[stageIndex][i + 1] = temp;
		}
	}
	//�����L���O�����킩
	CheckRankingIntegrity();
}
