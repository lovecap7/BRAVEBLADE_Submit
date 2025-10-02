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
    //�����̈ʒu
    Vector2 m_pos;
    //�I�΂�Ă邩
    bool m_isSelect;
    //�ҋ@
	bool m_isWait;
    //�n���h��
    int m_handle;
    //�T�C�Y
    float m_size;
	float m_sizeAngle;
};

