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
    //�o�b�N�摜
    int m_backHandle;
    //�e�L�X�g
    int m_textHandle;
};

