#pragma once
#include "ScoreUIBase.h"
#include <memory>
class Score;
class GameScoreUI :
    public ScoreUIBase
{
public:
    GameScoreUI(const std::weak_ptr<Score> score);
    ~GameScoreUI();
    void Update()override;
private:
    std::weak_ptr<Score> m_score;
};

