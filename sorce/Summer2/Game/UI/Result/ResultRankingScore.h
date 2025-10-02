#pragma once
#include "ResultScoreUI.h"
class ResultRankingScore :
    public ResultScoreUI
{
public:
    ResultRankingScore(int scoreValue, Vector2 basePos, const std::wstring& text,int nowScore);
    virtual ~ResultRankingScore();
	void Draw()const override;
private:
    //ランキングのハンドル
	int m_rankingHandle;
    //文字の色
    int m_textColor;
};

