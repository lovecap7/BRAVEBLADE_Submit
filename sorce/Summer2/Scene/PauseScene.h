#pragma once
#include "SceneBase.h"
#include "../General/StageIndex.h"
#include <memory>
#include <map>
class SceneController;
class MenuUI;
class Input;
class PauseScene :
    public SceneBase
{
public:
    PauseScene(SceneController& controller,Stage::StageIndex index);
    ~PauseScene();
    //派生クラスで実装を実装
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
private:
    //状態遷移
    using UpdateFunc_t = void(PauseScene::*)();
    using DrawFunc_t = void(PauseScene::*)();
    UpdateFunc_t m_update;
    DrawFunc_t m_draw;
    //出現中状態
    void AppearUpdate();
    //ノーマル状態
    void NormalUpdate();
    //消滅中状態
    void DisappearUpdate();
    //通常描画
    void NormalDraw();
    //途中描画
    void ShiftingDraw();
    //ゲームに戻る
    void RetrunGame();
    //ゲームを再スタート
    void RestartGame();
    //オプション
    void Option();
    //セレクトシーンへ
    void SelectStage();
private:
    int m_countFrame;
    enum class MenuIndex : int 
    {
        RetrunGame  = 1,
        RestartGame = 2,
        Option      = 3,
        SelectStage = 4
    };
    //メニューセレクト
    MenuIndex m_menuSelectIndex;
    //各メニューUI
    std::weak_ptr<MenuUI> m_pauseUI;
    std::map<MenuIndex,std::weak_ptr<MenuUI>> m_menuUIs;
    //初期化
    void InitDisappear();
    void InitUI();
    //メニュー選択
    void MenuSelect(Input& input);
    //ステージインデックス
	Stage::StageIndex m_stageIndex;
};

