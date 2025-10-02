#pragma once
#include "SceneBase.h"
#include "../General/StageIndex.h"
#include <memory>
#include <vector>
#include "../General/CSVDataLoader.h"
class SceneController;
class ResultScoreUI;
class SE;
class GameClearScene :
    public SceneBase
{
public:
    GameClearScene(SceneController& controller, Stage::StageIndex index);
    ~GameClearScene();
    //派生クラスで実装を実装
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
private:
    //状態遷移
    using UpdateFunc_t = void(GameClearScene::*)();
    using DrawFunc_t = void(GameClearScene::*)();
    UpdateFunc_t m_update;
    DrawFunc_t m_draw;
    //出現中状態
    void AppearUpdate();
    //リザルト１状態
    void Result1Update();
    //リザルト2状態
    void Result2Update();
    //消滅中状態
    void DisappearUpdate();
    //通常描画
    void NormalDraw();
    //UIの準備
    void InitResult1UI();
    void InitResult2UI();
private:
    int m_countFrame;
    //ステージの番号
    Stage::StageIndex m_stageIndex;
    //スコアUIのデータ
    std::vector<ResultScoreUIData> m_scoreUiData;
    //スコアUI
	std::vector<std::weak_ptr<ResultScoreUI>> m_scoreUIList;
    //加算SE
    std::weak_ptr<SE> m_addScoreSE;
};

