#pragma once
#include <map>
#include <memory>
#include <string>
#include "../General/StageIndex.h"
class Score;
//ゲームの進捗度など記録すべきものを記録し
//ゲームの開始時にロードし、ゲーム終了時に記録をするシングルトンクラス
class SaveDataManager final
{
private:
	//シングルトンの準備
	SaveDataManager() = default;
	~SaveDataManager() = default;
	//コピー禁止
	SaveDataManager(const SaveDataManager&) = delete;
	SaveDataManager& operator = (const SaveDataManager&) = delete;
	//ムーブ禁止
	SaveDataManager(SaveDataManager&&) = delete;
	SaveDataManager& operator = (SaveDataManager&&) = delete;
public:
	//インスタンスを取得
	static SaveDataManager& GetInstance()
	{
		static SaveDataManager instance;
		return instance;
	}
	//初期化
	void Init();
	//更新
	void Update();
	//終了処理
	void End();

	//ロード
	void Load();
	//記録
	void Save();
	//データを初期化して最初から
	void NewGame();

	//クリアしたステージを記録
	void SaveClearStage(Stage::StageIndex stageIndex);
	//そのステージをクリアしたのか
	bool IsClearStage(Stage::StageIndex stageIndex);
	//総プレイ時間
	unsigned int GetTotalPlayTime()const { return m_totalPlayTime; };
	//スコア
	std::weak_ptr<Score> GetScore() { return m_score; };
	//日付と時間
	std::wstring GetLastSaveTimeAndDate() { return m_lastSaveTime; };
	//ステージ全開放
	void AllClearStage();
private:
	//3ステージ文の初回クリア記録を持つ(ステージの進捗度に使う)
	std::map<Stage::StageIndex, bool>m_isClearStage;
	//プレイ時間
	unsigned int m_totalPlayTime;
	//スコア
	std::shared_ptr<Score> m_score;
	//日付と時間
	std::wstring m_lastSaveTime;
private:
	//日付と時間
	std::wstring GetNowTimeAndDate();
};

