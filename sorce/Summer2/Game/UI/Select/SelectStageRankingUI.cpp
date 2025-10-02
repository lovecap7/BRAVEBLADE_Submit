#include "SelectStageRankingUI.h"
#include "../UIManager.h"
#include "../../../General/CSVDataLoader.h"
#include "../../../SaveData/SaveDataManager.h"
#include "../../GameRule/Score.h"
#include <string>
#include <format>

namespace
{
	//ランキングの文字の座標
	constexpr int kRankingTextPosX = 220;
	constexpr int kRankingTextPosY = 160;
	//ランキングの座標
	constexpr int kRankingNumPosX = 200;
	constexpr int kRankingNumPosY = 240;
	//スコアの座標
	constexpr int kScorePosX = 260;
	constexpr int kScorePosY = 230;
	//Y軸の間隔
	constexpr int kIntervalY = 70;
}

SelectStageRankingUI::SelectStageRankingUI(Stage::StageIndex index):
	UIBase(),
	m_textHandle(UIManager::GetInstance().GetTextHandle("源直ゴシック32")),
	m_stageIndex(index)
{
	SaveDataManager& saveData = SaveDataManager::GetInstance();
	if (saveData.GetScore().expired())return;
	auto score = saveData.GetScore().lock();
	m_stageScore[Stage::StageIndex::Stage1] = score->GetHighScore(Stage::StageIndex::Stage1);
	m_stageScore[Stage::StageIndex::Stage2] = score->GetHighScore(Stage::StageIndex::Stage2);
	m_stageScore[Stage::StageIndex::Stage3] = score->GetHighScore(Stage::StageIndex::Stage3);
}

SelectStageRankingUI::~SelectStageRankingUI()
{
}

void SelectStageRankingUI::Update()
{
	
}

void SelectStageRankingUI::Draw() const
{
	DrawStringToHandle(kRankingTextPosX, kRankingTextPosY, L"Ranking", 0xffff33, m_textHandle);
	auto& uiManager = UIManager::GetInstance();
	for(auto i = 0; i < static_cast<int>(Stage::StageIndex::StageNum); ++i)
	{
		int handle = -1;
		if		(i == 0)handle = uiManager.GetImageHandle("1st");
		else if (i == 1)handle = uiManager.GetImageHandle("2nd");
		else if (i == 2)handle = uiManager.GetImageHandle("3rd");
		DrawRotaGraph(kRankingNumPosX, kRankingNumPosY + (i * kIntervalY), 0.3, 0.0, handle, true);

		std::wstring score = std::format(L"{:06}", m_stageScore.at(m_stageIndex)[i]);
		//テキスト
		DrawStringToHandle(kScorePosX, kScorePosY + (i * kIntervalY), score.c_str(), 0xffffff, m_textHandle);
	}
}

void SelectStageRankingUI::SetStageIndex(Stage::StageIndex index)
{
	m_stageIndex = index;
}
