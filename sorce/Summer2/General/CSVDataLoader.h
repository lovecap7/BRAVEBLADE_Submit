#pragma once
#include <DxLib.h>
#include <string>
#include <vector>
#include <array>
#include "CSVDatas.h"

class CSVDataLoader
{
public:
    CSVDataLoader();
	virtual ~CSVDataLoader();

    /// <summary>
    /// CSV�t�@�C������I�u�W�F�N�g��Tranceform�����擾
    /// </summary>
    /// <param name="fileName">csv</param>
    /// <returns></returns>
    std::vector<ActorData> LoadActorDataCSV(const char* fileName);

    /// <summary>
    /// �U���f�[�^�̓ǂݎ��
    /// </summary>
    /// <param name="fileName"></param>
    /// <param name="name"></param>
    /// <returns></returns>
    std::vector<AttackData> LoadAttackDataCSV();

    /// <summary>
    /// �A�N�^�[���̃X�R�A�f�[�^�̓ǂݎ��
    /// </summary>
    /// <param name="fileName"></param>
    /// <returns></returns>
    std::vector<ScoreData> LoadActorScoreDataCSV();

    /// <summary>
    /// �n�C�X�R�A�f�[�^�̓ǂݎ��
    /// </summary>
    /// <param name="fileName"></param>
    /// <returns></returns>
    std::array<std::array<int,3>, static_cast<int>(Stage::StageIndex::StageNum)> LoadHighScoreDataCSV();

    /// <summary>
    /// ���U���g�X�R�AUI���̓ǂݎ��
    /// </summary>
    /// <returns></returns>
    std::vector <ResultScoreUIData> LoadResultScoreUIDataCSV();

    /// <summary>
    /// UI���̓ǂݎ��
    /// </summary>
    /// <returns></returns>
    std::vector <UIData> LoadUIDataCSV(const char* fileName);

    /// <summary>
    /// �Z�[�u�f�[�^�̓ǂݎ��
    /// </summary>
    /// <returns></returns>
    SaveDatas LoadSaveDataCSV();
    /// <summary>
    /// ���ʂ̓ǂݎ��
    /// </summary>
    /// <returns></returns>
    VolumeData LoadVolumeDataCSV();
private:
    const std::vector<std::vector<std::string>> GetStringList(const char* fileName,int elementNum);
    const std::vector<std::vector<std::wstring>> GetWStringList(const char* fileName,int elementNum);
};