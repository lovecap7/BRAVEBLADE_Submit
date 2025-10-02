#include "SaveDataManager.h"
#include "../General/CSVDataLoader.h"
#include "../General/CSVDataSaver.h"
#include "../Game/GameRule/Score.h"
#include <ctime>
#include <sstream>
#include <iomanip>

void SaveDataManager::Init()
{
	//�X�R�A
	m_score = std::make_shared<Score>();
	//�X�R�A�̏�����
	m_score->Init();
	//�f�[�^�̃��[�h
	Load();
}

void SaveDataManager::Update()
{
	//�v���C���Ԃ����Z
	++m_totalPlayTime;
}

void SaveDataManager::End()
{
	Save();
}
void SaveDataManager::Load()
{
	auto csvLoader = std::make_shared<CSVDataLoader>();
	auto saveDatas = csvLoader->LoadSaveDataCSV();
	//�f�[�^�����o��
	m_isClearStage[Stage::StageIndex::Stage1] = saveDatas.stage1Clear;
	m_isClearStage[Stage::StageIndex::Stage2] = saveDatas.stage2Clear;
	m_isClearStage[Stage::StageIndex::Stage3] = saveDatas.stage3Clear;
	m_totalPlayTime							  = saveDatas.totalPlayTime;
	m_lastSaveTime							  = saveDatas.nowTime;
}

void SaveDataManager::Save()
{
	//�L�^����f�[�^�����
	SaveDatas saveDatas;
	saveDatas.stage1Clear = m_isClearStage[Stage::StageIndex::Stage1];
	saveDatas.stage2Clear = m_isClearStage[Stage::StageIndex::Stage2];
	saveDatas.stage3Clear = m_isClearStage[Stage::StageIndex::Stage3];
	saveDatas.totalPlayTime = m_totalPlayTime;
	saveDatas.nowTime = GetNowTimeAndDate();
	//CSV�ɏ�������
	auto csvSaver = std::make_shared<CSVDataSaver>();
	csvSaver->SaveDataToCSV(saveDatas);
}

void SaveDataManager::NewGame()
{
	//�S�Ẵf�[�^���폜����
	m_isClearStage[Stage::StageIndex::Stage1] = false;
	m_isClearStage[Stage::StageIndex::Stage2] = false;
	m_isClearStage[Stage::StageIndex::Stage3] = false;
	m_totalPlayTime							  = 0;
	m_lastSaveTime							  = GetNowTimeAndDate();
	//�X�R�A���폜
	m_score->NewGame();
	//CSV�ɏ�������
	Save();
}

void SaveDataManager::SaveClearStage(Stage::StageIndex stageIndex)
{
	//�X�R�A�X�V
	m_score->UpdateScore(stageIndex);
	//�n�C�X�R�A��ۑ�
	m_score->SaveHighScore();
	//�N���A����
	m_isClearStage[stageIndex] = true;
	//�L�^
	Save();
}

bool SaveDataManager::IsClearStage(Stage::StageIndex stageIndex)
{
	return m_isClearStage[stageIndex];
}

void SaveDataManager::AllClearStage()
{
	m_isClearStage[Stage::StageIndex::Stage1] = true;
	m_isClearStage[Stage::StageIndex::Stage2] = true;
	m_isClearStage[Stage::StageIndex::Stage3] = true;
	//�L�^
	Save();
}

std::wstring SaveDataManager::GetNowTimeAndDate()
{
	// ���ݎ������擾
	std::time_t t = std::time(nullptr);
	std::tm tm;
	localtime_s(&tm, &t);

	//wstring�ɕϊ�(��: 2025-08-17 12:34:56)
	std::wostringstream woss;
	woss << std::put_time(&tm, L"%Y-%m-%d %H:%M:%S");
	std::wstring nowTime = woss.str();
	return nowTime;
}
