#pragma once
#include <vector>
#include <memory>
#include "../General/StageIndex.h"

class GameCamera;
class ActorManager;
class UIManager;
class Timer;
class TutorialDirecter;
class GameManager
{
public:
    GameManager();
    ~GameManager();
    void Init(Stage::StageIndex index = Stage::StageIndex::Stage1);
    void Update();
    void Draw()const;
    void End();
    //リスタート
    void Restart(Stage::StageIndex index = Stage::StageIndex::Stage1);
    //ゲームオーバー
    bool IsGameover() { return m_isGameover; };
    //ゲームクリア
    bool IsGameClear() { return m_isGameClear; };
	//リザルト
    bool IsResult() { return m_isResult; };
    //コンテニュー
    void Continue();
private:
	//アクターマネージャー
	std::shared_ptr<ActorManager> m_actorManager;
    //カメラ
    std::shared_ptr<GameCamera> m_camera;
    //タイマー
    std::shared_ptr<Timer> m_timer;
    //チュートリアル
    std::shared_ptr<TutorialDirecter> m_tutorialDirecter;
private:
    //ゲームオーバーフラグ
    bool m_isGameover;
    //ゲームクリアフラグ
    bool m_isGameClear;
    //リザルトフラグ
	bool m_isResult;
    //影
    int m_shadowMapHandle;
    //ライトハンドル
	std::vector<int> m_lightHandles;
    //デバッグモード用
    bool m_isUpdateStop = false;
private:
    //ライトの初期化
    void InitLight();
    //シャドウマップの初期化
    void InitShadow();
    //描画範囲更新
    void UpdateShadowDrawArea();
    //ゲーム開始時の処理
    void UpdateGameStart();
};
