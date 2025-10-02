#include "ResultUI.h"
#include <DxLib.h>
#include <cassert>

namespace
{
	//�J�n�ʒuX
	constexpr float kStartPosX = -200.0f;
	//�I���ʒuX
	constexpr float kEndPosX = 0.0f;
	//Y���W
	constexpr float kPosY = 0.0f;
	//lerp�̊���
	constexpr float kLerpRate = 0.1f;
}

ResultUI::ResultUI():
	m_handle(LoadGraph(L"Data/UI/Result/Results.png")),
	m_pos{ kStartPosX, kPosY }
{
	assert(m_handle != -1);
}

ResultUI::~ResultUI()
{
	DeleteGraph(m_handle);
}

void ResultUI::Update()
{
	m_pos.x = MathSub::Lerp(m_pos.x, kEndPosX, kLerpRate);
}

void ResultUI::Draw() const
{
	DrawGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_handle, true);
}
