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
		//���t���[����
		++m_time;
		//�~���b���X�V�i60�t���[�� = 1�b�Ɖ���j
		++m_millisecond;
		if (m_millisecond >= kMillisecond)
		{
			m_millisecond = 0;
			++m_seconds;

			// �b��60�𒴂����番�ɌJ��グ
			if (m_seconds >= kSeconds)
			{
				m_seconds = 0;
				++m_minutes;
			}
		}
	}
}
