#pragma once
#include "UIBase.h"
#include "../../General/StageIndex.h"
#include "../../General/Math/MyMath.h"
class SaveDataUI :
    public UIBase
{
public:
    SaveDataUI();
    ~SaveDataUI();
    void Update()override;
    void Draw() const override;
private:
    void DrawClearStage(Vector2 pos,Stage::StageIndex stageIndex) const;
    //バック画像
    int m_backHandle;
    //テキスト
    int m_textHandle;
};

