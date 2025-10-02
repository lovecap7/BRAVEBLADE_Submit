#pragma once
#include "UIBase.h"
#include "../../General/Math/MyMath.h"
#include <string>
class MenuUI :
    public UIBase
{
public:
    MenuUI(Vector2 pos, int handle);
    ~MenuUI();
    void Update()override;
    void Draw() const override;
    void SetIsSelect(bool isSelect) { m_isSelect = isSelect; };
    bool IsSelect() const{ return m_isSelect; };
    void SetIsWait(bool isWait) { m_isWait = isWait; };
    bool IsWait() const { return m_isWait; };
private:
    //自分の位置
    Vector2 m_pos;
    //選ばれてるか
    bool m_isSelect;
    //待機
	bool m_isWait;
    //ハンドル
    int m_handle;
    //サイズ
    float m_size;
	float m_sizeAngle;
};

