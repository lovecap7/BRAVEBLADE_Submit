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
	//初期化
	void Init();
	//更新
	void UpdateScore(Stage::StageIndex index);
	//スコア
	int GetScore()const;
	int GetTimeScore()const	{ return m_timeScore; };
	int GetKillScore()const	{ return m_killScore; };
	int GetItemScore()const { return m_itemScore; };
	int GetHPScore()const { return m_hpScore; };
	std::array<int, static_cast<int>(Stage::StageIndex::StageNum)> GetHighScore(Stage::StageIndex index)const;
	//スコア加算
	void AddTimeScore(int time);
	void AddKillOrItemScore(std::string dataName);
	void AddHPScore(std::weak_ptr<HitPoints> hp);
	//スコアを保存
	void SaveHighScore();
	//全てのステージのスコアデータを削除
	void NewGame();
private:
	//ランキングの並びが正しいかチェック
	void CheckRankingIntegrity();
	//ランキングの更新
	void UpdateRanking(Stage::StageIndex index, int newScore);
private:
	//現在のスコア
	int m_nowScore;	
	//時間スコア
	int m_timeScore;
	//撃破スコア(敵を倒したときのスコア)
	int m_killScore;
	//アイテムスコア
	int m_itemScore;
	//体力スコア
	int m_hpScore;
	//ハイスコア
	std::array<std::array<int, 3>, static_cast<int>(Stage::StageIndex::StageNum)> m_highScore;
	//スコアデータ
	std::vector<ScoreData> m_acotrScoreData;
	//体力スコアデータ
	int m_hpScoreData;
	//タイマースコアデータ
	int m_timeScoreData;
	//タイマーの減少量データ
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


