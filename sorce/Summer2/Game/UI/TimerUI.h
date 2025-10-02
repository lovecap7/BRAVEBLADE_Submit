#pragma once
#include "UIBase.h"
#include <memory>

class ActorManager;
class Timer;
class TimerUI :
    public UIBase
{
public:
    TimerUI(const std::weak_ptr<Timer> timer);
    ~TimerUI();
    void Update()override;
    void Draw() const override;
private:
    std::weak_ptr<Timer> m_timer;
    //ƒnƒ“ƒhƒ‹
    int m_handle;
};

