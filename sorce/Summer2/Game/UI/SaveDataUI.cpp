#include "SaveDataUI.h"
#include "UIManager.h"
#include "../../General/game.h"
#include <DxLib.h>
#include "../../SaveData/SaveDataManager.h"

namespace
{
	//背景位置
	constexpr int kTextBackPosX = Game::kScreenCenterX + 250;
	constexpr int kTextBackPosY = Game::kScreenCenterY - 100;
	//時間位置
	constexpr int kTextBasePosX = Game::kScreenCenterX - 65;
	constexpr int kTextClearStage1PosY = kTextBackPosY - 80;
	constexpr int kTextClearStage2PosY = kTextBackPosY - 38;
	constexpr int kTextClearStage3PosY = kTextBackPosY + 2;
	constexpr int kTextPlayTimePosY = kTextBackPosY + 44;
	constexpr int kTextLastSaveTimeAndDatePosY = kTextBackPosY + 85;
	//分
	constexpr int kMinutes = 3600;
	//時
	constexpr int kHourss = 60;
}

SaveDataUI::SaveDataUI():
	UIBase(),
	m_backHandle(UIManager::GetInstance().GetImageHandle("SaveDataBack")),
	m_textHandle(UIManager::GetInstance().GetTextHandle("源直ゴシック32"))
{

}

SaveDataUI::~SaveDataUI()
{
}

void SaveDataUI::Update()
{
}

void SaveDataUI::Draw() const
{
	if (!m_isDraw)return;
	DrawRotaGraph(kTextBackPosX, kTextBackPosY, 1.0, 0.0, m_backHandle, true);
	//クリアしたステージ
	DrawClearStage(Vector2{ kTextBasePosX ,kTextClearStage1PosY },Stage::StageIndex::Stage1);
	DrawClearStage(Vector2{ kTextBasePosX ,kTextClearStage2PosY },Stage::StageIndex::Stage2);
	DrawClearStage(Vector2{ kTextBasePosX ,kTextClearStage3PosY },Stage::StageIndex::Stage3);

	//トータルプレイ時間
	auto totalTime = SaveDataManager::GetInstance().GetTotalPlayTime();
	auto minutes = (totalTime / kMinutes) % kHourss;
	auto hours	 = (totalTime / kMinutes) / kHourss;
	std::wstring timeText = L"総プレイ時間 : " + std::to_wstring(hours) + L"時間" + std::to_wstring(minutes) + L"分";
	DrawStringToHandle(kTextBasePosX, kTextPlayTimePosY, timeText.c_str(), 0xffffff, m_textHandle);
	auto lastSaveText = L"前回のプレイ : " + SaveDataManager::GetInstance().GetLastSaveTimeAndDate();
	//日付と時間
	DrawStringToHandle(kTextBasePosX, kTextLastSaveTimeAndDatePosY, lastSaveText.c_str(), 0xffffff, m_textHandle);
}

void SaveDataUI::DrawClearStage(Vector2 pos, Stage::StageIndex stageIndex) const
{
	auto isClear = SaveDataManager::GetInstance().IsClearStage(stageIndex);
	std::wstring stageClearText = L"ステージ" + std::to_wstring(static_cast<int>(stageIndex) + 1);
	if (isClear)
	{
		stageClearText += L" : クリア";
		DrawStringToHandle(pos.x, pos.y, stageClearText.c_str(), 0xff6666, m_textHandle);
	}
	else
	{
		stageClearText += L" : 未クリア";
		DrawStringToHandle(pos.x, pos.y, stageClearText.c_str(), 0xffffff, m_textHandle);
	}
	
}
