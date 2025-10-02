#include "ScreenModeUI.h"
#include "../../../Main/Application.h"
#include "../UIManager.h"
#include "../../../General/game.h"
#include <DxLib.h>

ScreenModeUI::ScreenModeUI(Vector2 pos):
	m_pos(pos)
{
	m_isWindowMode = Application::GetInstance().IsWindowMode();
	auto& uiManager = UIManager::GetInstance();
	//ウィンドウモード
	m_windowModeHandle = uiManager.GetImageHandle("WindowMode");
	//フルスクリーン
	m_fullScreenModeHandle = uiManager.GetImageHandle("FullScreenMode");
	//背景
	m_backgroundHandle = uiManager.GetImageHandle("OptionBack");
}

ScreenModeUI::~ScreenModeUI()
{
}

void ScreenModeUI::Update()
{
	m_isWindowMode = Application::GetInstance().IsWindowMode();
}

void ScreenModeUI::Draw() const
{
	DrawGraph(0, 0, m_backgroundHandle, true);
	if (m_isWindowMode)
	{
		DrawRotaGraph(m_pos.x, m_pos.y, 1.0, 0.0, m_windowModeHandle, true);
	}
	else
	{
		DrawRotaGraph(m_pos.x, m_pos.y, 1.0, 0.0, m_fullScreenModeHandle, true);
	}
}
