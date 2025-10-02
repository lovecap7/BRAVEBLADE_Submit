#include "TimerUI.h"
#include "../GameRule/Timer.h"
#include "../../General/game.h"
#include "../Actors/ActorManager.h"
#include "../UI/UIManager.h"
#include <DxLib.h>

namespace
{
	//�X�R�A�̌���
	constexpr int kDigitNum = 2;
	//�傫��
	constexpr float kScale = 0.1f;
	//��
	constexpr int kImageWidth = 256;
	constexpr int kImageHeight = 256;
	//�^�C�}�[��X���W
	constexpr int kMinSecPosX = (Game::kScreenWidth - 100);
	constexpr int kSecPosX = (Game::kScreenWidth - 130);
	constexpr int kMinPosX = (Game::kScreenWidth - 160);
	//�^�C�}�[��Y���W
	constexpr int kTitlePosY = 30;
	//�^�C�}�[��1���̕�
	constexpr int kDigitMargin = 13;
}

TimerUI::TimerUI(const std::weak_ptr<Timer> timer):
	UIBase(),
	m_timer(timer),
	m_handle(UIManager::GetInstance().GetImageHandle("Timer"))
{
}

TimerUI::~TimerUI()
{
}


void TimerUI::Update()
{
	if (m_timer.expired())
	{
		m_isDelete = true;
		return;
	}
}

void TimerUI::Draw() const
{
	if (m_timer.expired())return;
	if (!m_isDraw)return;
	auto time = m_timer.lock();
	int minSec = time->GetMillisecond();
	int sec = time->GetSeconds();
	int min = time->GetMinutes();
	//�e��
	int digits[kDigitNum];
	for (int i = 0;i < kDigitNum;++i)
	{
		//���o���ĕۑ�
		digits[i] = minSec % 10;
		//����������
		minSec = minSec / 10;
		//�؂�����v�Z����
		int sizeX, sizeY;
		GetGraphSize(m_handle, &sizeX, &sizeY);//�摜�T�C�Y
		int cutX = digits[i] % (sizeX / kImageWidth);//��
		int cutY = digits[i] / (sizeX / kImageWidth);//�c
		//�`��
		DrawRectRotaGraphFast(kMinSecPosX - i * kDigitMargin, kTitlePosY,
			kImageWidth * cutX,
			kImageHeight * cutY,
			kImageWidth, kImageHeight,
			kScale, 0.0f, m_handle, true, false);
	}
	for (int i = 0;i < kDigitNum;++i)
	{
		//���o���ĕۑ�
		digits[i] = sec % 10;
		//����������
		sec = sec / 10;
		//�؂�����v�Z����
		int sizeX, sizeY;
		GetGraphSize(m_handle, &sizeX, &sizeY);//�摜�T�C�Y
		int cutX = digits[i] % (sizeX / kImageWidth);//��
		int cutY = digits[i] / (sizeX / kImageWidth);//�c
		//�`��
		DrawRectRotaGraphFast(kSecPosX - i * kDigitMargin, kTitlePosY,
			kImageWidth * cutX,
			kImageHeight * cutY,
			kImageWidth, kImageHeight,
			kScale, 0.0f, m_handle, true, false);
	}
	for (int i = 0;i < kDigitNum;++i)
	{
		//���o���ĕۑ�
		digits[i] = min % 10;
		//����������
		min = min / 10;
		//�؂�����v�Z����
		int sizeX, sizeY;
		GetGraphSize(m_handle, &sizeX, &sizeY);//�摜�T�C�Y
		int cutX = digits[i] % (sizeX / kImageWidth);//��
		int cutY = digits[i] / (sizeX / kImageWidth);//�c
		//�`��
		DrawRectRotaGraphFast(kMinPosX - i * kDigitMargin, kTitlePosY,
			kImageWidth * cutX,
			kImageHeight * cutY,
			kImageWidth, kImageHeight,
			kScale, 0.0f, m_handle, true, false);
	}
}
