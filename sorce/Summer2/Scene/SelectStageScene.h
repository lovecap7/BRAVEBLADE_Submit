#pragma once
#include "SceneBase.h"
#include <memory>
#include <map>
#include <string>
#include <array>
#include "../General/Math/MyMath.h"
#include "../General/StageIndex.h"
class Input;
class SelectStagePlayer;
class SelectStageCamera;
class SceneController;
class StageFigure;
class SelectStageBackUI;
class SelectStageRankingUI;
class LeftArrowUI;
class SelectStageScene :
    public SceneBase
{
public:
    SelectStageScene(SceneController& controller, Stage::StageIndex index);
    ~SelectStageScene();
    /// <summary>
    /// 派生クラスで実装を実装
    /// virtual はこれを継承するかもしれないから
    /// overrideは明示的にオーバーライドをエディタに示すため
    /// </summary>
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
    //ステージセレクト
    void SelectStageIndex(Input& input);
private:
    //決定
    bool m_isDecide;
    //カメラ
    std::unique_ptr<SelectStageCamera> m_camera;
    //モデル
    std::unique_ptr<SelectStagePlayer> m_player;
    //ステージの場所
    std::map<Stage::StageIndex,Vector3> m_stagePos;
    //ステージのインデックス
    int m_stageIndex;
    //ステージのフィギュア
	std::array<std::shared_ptr<StageFigure>,static_cast<int>(Stage::StageIndex::StageNum)> m_stageFigures;
    //背景UI
    std::weak_ptr<SelectStageBackUI> m_stage1Back;
    std::weak_ptr<SelectStageBackUI> m_stage2Back;
	//ランキングUI
    std::weak_ptr<SelectStageRankingUI> m_stageRankingUI;
	//矢印UI
	std::weak_ptr<LeftArrowUI> m_leftArrowUI;
	std::weak_ptr<LeftArrowUI> m_rightArrowUI;
    //解放している最終ステージのインデックス
    Stage::StageIndex m_unlockStageIndex;
    //背景を変える
    void ChangeBack();
    //現状解放されているステージまでしか選べなくするための処理
    void NowUnlockStage();
};

