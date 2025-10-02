#pragma once
#include "SceneBase.h"
#include "../General/StageIndex.h"
#include <memory>
#include <map>
class SceneController;
class MenuUI;
class Input;
class GameoverScene :
    public SceneBase
{
public:
    GameoverScene(SceneController& controller, Stage::StageIndex index);
    ~GameoverScene();
    //派生クラスで実装を実装
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
private:
    //状態遷移
    using UpdateFunc_t = void(GameoverScene::*)();
    using DrawFunc_t = void(GameoverScene::*)();
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
    //UI初期化
    void InitUI();
    //メニューセレクト
    void MenuSelect(Input& input);
    //つづきから
    void Continue();
    //再スタート
    void RestartBase();
    //ステージセレクトへ
    void SelectStage();
private:
    int m_countFrame;
    enum class MenuIndex : int
    {
        Continue    = 1,
        Restart     = 2,
        SelectStage = 3
    };
    //メニューセレクト
    MenuIndex m_menuSelectIndex;
    //各メニューUI
    std::weak_ptr<MenuUI> m_gameoverUI;
    std::map<MenuIndex, std::weak_ptr<MenuUI>> m_menuUIs;
	//ステージの番号
    Stage::StageIndex m_stageIndex;
};

