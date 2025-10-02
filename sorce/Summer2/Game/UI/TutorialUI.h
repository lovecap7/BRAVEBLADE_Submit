#pragma once
#include "UIBase.h"
#include <memory>
#include <string>

class ActorManager;
class Timer;
class TutorialUI :
    public UIBase
{
public:
    TutorialUI(const std::wstring& text);
    ~TutorialUI();
    void Update()override;
    void Draw() const override;
private:
    //フレーム
    int m_countFrame;
    //表示している文字数
    int m_chatCount;
    //テキスト
    std::wstring m_text;
    //X座標
    int m_posX;
private:
    //状態遷移
    using UpdateFunc_t = void(TutorialUI::*)();
    UpdateFunc_t m_update;
    //出現中状態
    void AppearUpdate();
    //通常状態
    void NormalUpdate();
    //消滅中状態
    void DisappearUpdate();
    //ハンドル
    int m_textHandle;
    int m_tutorialMarkUI;
};


