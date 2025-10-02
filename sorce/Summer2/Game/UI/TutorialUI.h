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
    //�t���[��
    int m_countFrame;
    //�\�����Ă��镶����
    int m_chatCount;
    //�e�L�X�g
    std::wstring m_text;
    //X���W
    int m_posX;
private:
    //��ԑJ��
    using UpdateFunc_t = void(TutorialUI::*)();
    UpdateFunc_t m_update;
    //�o�������
    void AppearUpdate();
    //�ʏ���
    void NormalUpdate();
    //���Œ����
    void DisappearUpdate();
    //�n���h��
    int m_textHandle;
    int m_tutorialMarkUI;
};


