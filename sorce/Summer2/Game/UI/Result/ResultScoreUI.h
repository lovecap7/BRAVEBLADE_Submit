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
    //スコアの値
    int m_scoreValue;
    //ハンドル
    int m_textHandle;
    //フレームを数える
	int m_countFrame;
    //テキスト
    const std::wstring m_text;
};

