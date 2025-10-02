#pragma once
#include <memory>
#include <list>
#include <map>
#include <string>
class UIBase;
class Player;
class EnemyBase;
class Score;
class Timer;
class BossHPUI;
class GameScoreUI;
class TimerUI;
class UIManager final
{
private:
	//シングルトンの準備
	UIManager() = default;
	~UIManager() = default;
	//コピー禁止
	UIManager(const UIManager&) = delete;
	UIManager& operator = (const UIManager&) = delete;
	//ムーブ禁止
	UIManager(UIManager&&) = delete;
	UIManager& operator = (UIManager&&) = delete;
public:
	//インスタンスを取得
	static UIManager& GetInstance()
	{
		static UIManager instance;
		return instance;
	}
	//登録
	void Entry(std::shared_ptr<UIBase> ui);
	//解除
	void Exit(std::shared_ptr<UIBase> ui);
	void Init();
	void Update();
	//後ろ描画
	void BackDraw()const;
	//前描画
	void FrontDraw()const;
	void End();
	void Reset();

	//プレイヤー関連のUI
	void CreatePlayerUI(const std::weak_ptr<Player> player);
	//ボスUI
	std::weak_ptr<BossHPUI> CreateBossUI(const std::weak_ptr<EnemyBase> boss);
	//敵UI
	void CreateEnemyUI(const std::weak_ptr<EnemyBase> enemy);
	//スコア
	std::weak_ptr<GameScoreUI> CreateGameScoreUI(const std::weak_ptr<Score> score);
	//タイマー
	std::weak_ptr<TimerUI> CreateTimerUI(const std::weak_ptr<Timer> timer);
	//画像ハンドル取得
	int GetImageHandle(const std::string& name)const;
	//テキストハンドル取得
	int GetTextHandle(const std::string& name)const;
	//ハンドルロード
	void LoadHandle();
	//UIのハンドル削除
	void AllDeleteHandle();
	//更新を止める
	void StopUpdate() { m_isUpdate = false; };
	//更新をする
	void StartUpdate() { m_isUpdate = true; };
	//描画を止める
	void StopDraw() { m_isDraw = false; };
	//描画をする
	void StartDraw() { m_isDraw = true; };
	//各自UIの描画を止める
	void AllStopDraw();
	//各自UIの描画をする
	void AllStartDraw();
private:
	//UI
	std::list<std::shared_ptr<UIBase>> m_uis;
	//UIの削除
	void AllDeleteUIs();
	//削除予定のUIを削除
	void CheckDelete();
	//画像ハンドル
	std::map<std::string, int> m_imageHandles;
	//テキストハンドル
	std::map<std::string, int> m_textHandles;
	//更新をする
	bool m_isUpdate;
	//描画する
	bool m_isDraw;
};

