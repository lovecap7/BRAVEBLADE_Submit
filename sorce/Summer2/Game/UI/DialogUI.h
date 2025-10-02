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
    //選んでるテキスト
    void SelectYes();
    void SelectNo();
    bool IsYes();
private:
    //バック
    int m_backHandle;
    //テキストハンドル
    int m_texthandle;
    //テキスト
    std::wstring m_text;
    //Yes
    std::weak_ptr<MenuUI> m_yesUI;
    //No
    std::weak_ptr<MenuUI> m_noUI;
};

