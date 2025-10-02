#include "Timer.h"

namespace
{
	constexpr int kMaxTime = 999999;
	constexpr int kMillisecond = 60;
	constexpr int kSeconds = 60;
	constexpr int kMinutes = 60;
}

Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::Init()
{
	m_time = 0;
	m_millisecond = 0;
	m_seconds = 0;
	m_minutes = 0;
	m_isUpdate = true;
}

void Timer::Update()
{
	if (!m_isUpdate)return;
	if (m_time < kMaxTime)
	{
		//総フレーム数
		++m_time;
		//ミリ秒を更新（60フレーム = 1秒と仮定）
		++m_millisecond;
		if (m_millisecond >= kMillisecond)
		{
			m_millisecond = 0;
			++m_seconds;

			// 秒が60を超えたら分に繰り上げ
			if (m_seconds >= kSeconds)
			{
				m_seconds = 0;
				++m_minutes;
			}
		}
	}
}
