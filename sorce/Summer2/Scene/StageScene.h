#pragma once
#include "SceneBase.h"
#include "../General/StageIndex.h"
#include <vector>
#include <memory>

class SceneController;
class Input;
class GameManager;
class StageScene :
    public SceneBase
{
public:
    StageScene(SceneController& controller, Stage::StageIndex index = Stage::StageIndex::Stage1);
    ~StageScene();
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
    void Continue();
private:
    //ゲームマネージャー
    std::unique_ptr<GameManager> m_gameManager;
    //ステージ番号
    Stage::StageIndex m_stageIndex;
	//BGM初期化
    void InitBGM();
};

