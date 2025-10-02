#pragma once
#include <memory>
#include <vector>
#include <array>
#include "../../General/CSVDatas.h"
#include "../../General/StageIndex.h"
class HitPoints;
class CSVDataLoader;
class Score :
	public std::enable_shared_from_this<Score>
{
public:
	Score();
	~Score();
	//������
	void Init();
	//�X�V
	void UpdateScore(Stage::StageIndex index);
	//�X�R�A
	int GetScore()const;
	int GetTimeScore()const	{ return m_timeScore; };
	int GetKillScore()const	{ return m_killScore; };
	int GetItemScore()const { return m_itemScore; };
	int GetHPScore()const { return m_hpScore; };
	std::array<int, static_cast<int>(Stage::StageIndex::StageNum)> GetHighScore(Stage::StageIndex index)const;
	//�X�R�A���Z
	void AddTimeScore(int time);
	void AddKillOrItemScore(std::string dataName);
	void AddHPScore(std::weak_ptr<HitPoints> hp);
	//�X�R�A��ۑ�
	void SaveHighScore();
	//�S�ẴX�e�[�W�̃X�R�A�f�[�^���폜
	void NewGame();
private:
	//�����L���O�̕��т����������`�F�b�N
	void CheckRankingIntegrity();
	//�����L���O�̍X�V
	void UpdateRanking(Stage::StageIndex index, int newScore);
private:
	//���݂̃X�R�A
	int m_nowScore;	
	//���ԃX�R�A
	int m_timeScore;
	//���j�X�R�A(�G��|�����Ƃ��̃X�R�A)
	int m_killScore;
	//�A�C�e���X�R�A
	int m_itemScore;
	//�̗̓X�R�A
	int m_hpScore;
	//�n�C�X�R�A
	std::array<std::array<int, 3>, static_cast<int>(Stage::StageIndex::StageNum)> m_highScore;
	//�X�R�A�f�[�^
	std::vector<ScoreData> m_acotrScoreData;
	//�̗̓X�R�A�f�[�^
	int m_hpScoreData;
	//�^�C�}�[�X�R�A�f�[�^
	int m_timeScoreData;
	//�^�C�}�[�̌����ʃf�[�^
	int m_decTimeScoreData;
	//CSV
	std::unique_ptr<CSVDataLoader> m_csvLoader;
};
namespace ScoreDataName
{
	const std::string kHPScore			= "HPScore";
	const std::string kTimeScore		= "TimeScore";
	const std::string kDecTimeScore		= "DecTimeScore";
	const std::string kPurpleDinosaur	= "PurpleDinosaur";
	const std::string kSmallDragon		= "SmallDragon";
	const std::string kBomber			= "Bomber";
	const std::string kBossDragon		= "BossDragon";
	const std::string kBossMuscle		= "BossMuscle";
	const std::string kAttackUp			= "AttackUp";
	const std::string kDefenseUp		= "DefenseUp";
	const std::string kHeart			= "Heart";
	const std::string kUltGageUp		= "UltGageUp";
}


