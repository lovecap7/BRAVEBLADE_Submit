#pragma once
#include "../DissolveUIBase.h"
#include <memory>

class SelectStageBackUI :
    public DissolveUIBase
{
public:
    SelectStageBackUI(int mainHandle);
    ~SelectStageBackUI();
    void Update()override;
    void Draw() const override;
    void SetAppear(bool isAppear) { m_isAppear = isAppear; };
private:
    //èoåªèà
    bool m_isAppear;
};

