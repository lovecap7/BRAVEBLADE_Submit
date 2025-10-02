#include "SaveDataUI.h"
#include "UIManager.h"
#include "../../General/game.h"
#include <DxLib.h>
#include "../../SaveData/SaveDataManager.h"

namespace
{
	//�w�i�ʒu
	constexpr int kTextBackPosX = Game::kScreenCenterX + 250;
	constexpr int kTextBackPosY = Game::kScreenCenterY - 100;
	//���Ԉʒu
	constexpr int kTextBasePosX = Game::kScreenCenterX - 65;
	constexpr int kTextClearStage1PosY = kTextBackPosY - 80;
	constexpr int kTextClearStage2PosY = kTextBackPosY - 38;
	constexpr int kTextClearStage3PosY = kTextBackPosY + 2;
	constexpr int kTextPlayTimePosY = kTextBackPosY + 44;
	constexpr int kTextLastSaveTimeAndDatePosY = kTextBackPosY + 85;
	//��
	constexpr int kMinutes = 3600;
	//��
	constexpr int kHourss = 60;
}

SaveDataUI::SaveDataUI():
	UIBase(),
	m_backHandle(UIManager::GetInstance().GetImageHandle("SaveDataBack")),
	m_textHandle(UIManager::GetInstance().GetTextHandle("�����S�V�b�N32"))
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
	//�N���A�����X�e�[�W
	DrawClearStage(Vector2{ kTextBasePosX ,kTextClearStage1PosY },Stage::StageIndex::Stage1);
	DrawClearStage(Vector2{ kTextBasePosX ,kTextClearStage2PosY },Stage::StageIndex::Stage2);
	DrawClearStage(Vector2{ kTextBasePosX ,kTextClearStage3PosY },Stage::StageIndex::Stage3);

	//�g�[�^���v���C����
	auto totalTime = SaveDataManager::GetInstance().GetTotalPlayTime();
	auto minutes = (totalTime / kMinutes) % kHourss;
	auto hours	 = (totalTime / kMinutes) / kHourss;
	std::wstring timeText = L"���v���C���� : " + std::to_wstring(hours) + L"����" + std::to_wstring(minutes) + L"��";
	DrawStringToHandle(kTextBasePosX, kTextPlayTimePosY, timeText.c_str(), 0xffffff, m_textHandle);
	auto lastSaveText = L"�O��̃v���C : " + SaveDataManager::GetInstance().GetLastSaveTimeAndDate();
	//���t�Ǝ���
	DrawStringToHandle(kTextBasePosX, kTextLastSaveTimeAndDatePosY, lastSaveText.c_str(), 0xffffff, m_textHandle);
}

void SaveDataUI::DrawClearStage(Vector2 pos, Stage::StageIndex stageIndex) const
{
	auto isClear = SaveDataManager::GetInstance().IsClearStage(stageIndex);
	std::wstring stageClearText = L"�X�e�[�W" + std::to_wstring(static_cast<int>(stageIndex) + 1);
	if (isClear)
	{
		stageClearText += L" : �N���A";
		DrawStringToHandle(pos.x, pos.y, stageClearText.c_str(), 0xff6666, m_textHandle);
	}
	else
	{
		stageClearText += L" : ���N���A";
		DrawStringToHandle(pos.x, pos.y, stageClearText.c_str(), 0xffffff, m_textHandle);
	}
	
}
