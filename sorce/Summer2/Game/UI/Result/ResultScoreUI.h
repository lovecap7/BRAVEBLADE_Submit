#pragma once
#include "../ScoreUIBase.h"
#include "../../../General/Math/MyMath.h"
#include <string>

class Score;
class ResultScoreUI :
    public ScoreUIBase
{
public:
    ResultScoreUI(int scoreValue, Vector2 basePos, const std::wstring& text);
    virtual ~ResultScoreUI();
    void Update()override;
    void Draw()const override;
protected:
    //�X�R�A�̒l
    int m_scoreValue;
    //�n���h��
    int m_textHandle;
    //�t���[���𐔂���
	int m_countFrame;
    //�e�L�X�g
    const std::wstring m_text;
};

