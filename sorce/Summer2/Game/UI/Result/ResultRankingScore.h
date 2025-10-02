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
    //�����L���O�̃n���h��
	int m_rankingHandle;
    //�����̐F
    int m_textColor;
};

