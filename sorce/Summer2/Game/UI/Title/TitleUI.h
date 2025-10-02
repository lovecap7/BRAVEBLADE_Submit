#pragma once
#include "../DissolveUIBase.h"
#include <memory>

class TitleUI :
    public DissolveUIBase
{
public:
    TitleUI();
    ~TitleUI();
    void Update()override;
    void Draw() const override;
    void Reset();
private:
    //�t���[���𐔂���
    int m_countFrame;
    //����
    bool m_isDecide;
	//�o���������I��������ǂ���
    bool m_isAppeared;
    //�n���h��
    int m_titleBackHandle;
    int m_pressAnyBottunHandle;
    int m_pressAnyBottunBackHandle;
   
};

