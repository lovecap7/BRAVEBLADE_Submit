#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
#include <map>
#include <string>
class Input;
class TitleCamera;
class SceneController;
class Model;
class TitleUI;
class TitlePlayer;
class Actor;
class CSVDataLoader;
class MenuUI;
class SaveDataUI;
class DialogUI;

namespace
{
    constexpr int kMenuNum = 4;
}

class TitleScene :
    public SceneBase
{
public:
    TitleScene(SceneController& controller);
    ~TitleScene();
    /// <summary>
    /// 派生クラスで実装を実装
    /// virtual はこれを継承するかもしれないから
    /// overrideは明示的にオーバーライドをエディタに示すため
    /// </summary>
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override {};
    //ハンドルロード
    void LoadHandle();
    //配置
    void LoadStage(std::shared_ptr<CSVDataLoader> csvLoader);
    //ハンドル削除
    void AllDeleteHandle();
    //オブジェ削除
    void AllDeleteStage();
private:
    //カメラ
	std::unique_ptr<TitleCamera> m_camera;
    //プレイヤー
	std::shared_ptr<TitlePlayer> m_player;
    //描画用オブジェ
    std::vector<std::shared_ptr<Actor>> m_stageObjects;
    //ハンドル
    std::map<std::string, int> m_stageHandles;
    //影
    int m_shadowMapHandle;
    //ライトハンドル
    int m_lightHandle;
    enum class MenuIndex : int
    {
        Continue = 0,
        NewGame = 1,
        Option = 2,
        FinishGame = 3
    };
    //タイトルUI
    std::weak_ptr<TitleUI> m_titleUI;
    //メニューUI
    std::map<MenuIndex,std::weak_ptr<MenuUI>> m_menuUIs;
    //セーブデータUI
    std::weak_ptr<SaveDataUI> m_saveDataUI;
    //ダイヤログ
    std::weak_ptr<DialogUI> m_dialogUI;
    //メニューインデックス
    MenuIndex m_menuIndex;
    //決定
    bool m_isDecide;
private:
    //状態遷移
    using UpdateFunc_t = void(TitleScene::*)(Input& input);
    UpdateFunc_t m_update;
    //タイトルのみの更新
    void UpdateTitle(Input& input);
    //セレクト画面
    void UpdateSelectMenu(Input& input);
    //ダイアログ
    void UpdateDialog(Input& input);
private:
    //メニューセレクト処理
    void SelectMenu(Input& input);
    //続きから
    void InitContinue();
    void Continue();
    //初めから
    void InitNewGame();
    void NewGame();
    //設定
    void Option();
    //ゲーム終了
    void InitFinishGame();
    void FinishGame();
private:
    //ライトの初期化
    void InitLight();
    //シャドウマップの初期化
    void InitShadow();
    //シャドウの更新
    void UpdateShadow();
    //Actorの更新
    void UpdateCommon();
    //UIの準備、初期化
    void InitUIs(std::shared_ptr<CSVDataLoader>& csvLoader);
    //タイトル画面の初期化
    void InitTitle();
    //メニュー画面の初期化
    void InitSelectMenu();

};

