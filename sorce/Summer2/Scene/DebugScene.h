#pragma once
#include "SceneBase.h"
class Input;
class SceneController;
class DebugScene :
    public SceneBase
{
public:
    DebugScene(SceneController& controller);
    ~DebugScene();
    //派生クラスで実装を実装
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
private:
    //カーソル移動のためのインデックス
    int m_selectSceneIndex;
};

