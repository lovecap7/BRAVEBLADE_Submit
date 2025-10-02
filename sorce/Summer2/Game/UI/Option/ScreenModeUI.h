#pragma once
#include "../UIBase.h"
#include "../../../General/Math/MyMath.h"
class ScreenModeUI :
    public UIBase
{
public:
    ScreenModeUI(Vector2 pos);
    ~ScreenModeUI();
    void Update()override;
    void Draw() const override;
    void SetWindowMode(bool isWindow) { m_isWindowMode = isWindow; };
private:
    bool m_isWindowMode;
    int m_windowModeHandle;
    int m_fullScreenModeHandle;
	int m_backgroundHandle;
	Vector2 m_pos;
};

