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
    //フレームを数える
    int m_countFrame;
    //決定
    bool m_isDecide;
	//出現処理が終わったかどうか
    bool m_isAppeared;
    //ハンドル
    int m_titleBackHandle;
    int m_pressAnyBottunHandle;
    int m_pressAnyBottunBackHandle;
   
};

