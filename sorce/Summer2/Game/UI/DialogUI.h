#pragma once
#include "UIBase.h"
#include <string>
#include <memory>
class MenuUI;
class DialogUI :
    public UIBase
{
public:
    DialogUI();
    ~DialogUI();
    void Init()override;
    void Update()override;
    void Draw() const override;
    void SetText(const std::wstring& text);
    //�I��ł�e�L�X�g
    void SelectYes();
    void SelectNo();
    bool IsYes();
private:
    //�o�b�N
    int m_backHandle;
    //�e�L�X�g�n���h��
    int m_texthandle;
    //�e�L�X�g
    std::wstring m_text;
    //Yes
    std::weak_ptr<MenuUI> m_yesUI;
    //No
    std::weak_ptr<MenuUI> m_noUI;
};

