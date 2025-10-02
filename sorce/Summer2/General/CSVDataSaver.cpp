#include "CSVDataSaver.h"
#include "StringUtil.h"
#include <iostream>
#include <fstream>
#include <list>
#include <cassert>
#include "../Game/GameRule/Score.h"

CSVDataSaver::CSVDataSaver()
{
}

CSVDataSaver::~CSVDataSaver()
{
}

void CSVDataSaver::SaveScoreDataToCSV(std::shared_ptr<Score> score)
{
    //�t�@�C�����J��(�Ȃ��ꍇ�͍쐬�����)
    std::ofstream file(kHighScoreDataPath);
    //�t�@�C�����ǂݍ��߂Ȃ�������
    if (!file)
    {
        assert("�t�@�C���̓ǂݍ��݂Ɏ��s");
    }
    
    //�w�b�_�[����������
    file << "1��,2��,3��,Stage\n";
    //�e�X�e�[�W�̃����L���O���L�^
    for (int i = static_cast<int>(Stage::StageIndex::Stage1);i < static_cast<int>(Stage::StageIndex::StageNum);++i)
    {
        auto highScore = score->GetHighScore(static_cast<Stage::StageIndex>(i));
        //�f�[�^��������
        file << highScore[0] << ","
             << highScore[1] << ","
             << highScore[2] << ","
             << "Stage" << i + 1 << std::endl;
    }
    //����
    file.close();
}

void CSVDataSaver::SaveDataToCSV(SaveDatas saveDatas)
{
    //�t�@�C�����J��(�Ȃ��ꍇ�͍쐬�����)
    std::ofstream file(kSaveDataPath);
    //�t�@�C�����ǂݍ��߂Ȃ�������
    if (!file)
    {
        assert("�t�@�C���̓ǂݍ��݂Ɏ��s");
    }

    //�w�b�_�[����������
    file << "�X�e�[�W1�̃N���A,�X�e�[�W2�̃N���A,�X�e�[�W3�̃N���A,���v���C����,���t�Ǝ���\n";

    //�f�[�^��������
    file << static_cast<int>(saveDatas.stage1Clear) << ","//�X�e�[�W1�̃N���A
         << static_cast<int>(saveDatas.stage2Clear) << ","//�X�e�[�W2�̃N���A
         << static_cast<int>(saveDatas.stage3Clear) << ","//�X�e�[�W3�̃N���A
         << saveDatas.totalPlayTime                 << ","//���v���C����  
         << StringUtil::WstringToString(saveDatas.nowTime)//���t�Ǝ���
         << std::endl;
    //����
    file.close();
}

void CSVDataSaver::SaveVolumeDataToCSV(VolumeData volumeDatas)
{
    //�t�@�C�����J��(�Ȃ��ꍇ�͍쐬�����)
    std::ofstream file(kVolumeDataPath);
    //�t�@�C�����ǂݍ��߂Ȃ�������
    if (!file)
    {
        assert("�t�@�C���̓ǂݍ��݂Ɏ��s");
    }

    //�w�b�_�[����������
    file << "�}�X�^�[,BGM,SE,Voice\n";

    //�f�[�^��������
    file << static_cast<int>(volumeDatas.masterVolume) << ","       //�}�X�^�[�{�����[��
        << static_cast<int>(volumeDatas.bgmVolume) << ","           //BGM�{�����[��
        << static_cast<int>(volumeDatas.seVolume) << ","            //SE�{�����[��
        << static_cast<int>(volumeDatas.voiceVolume) << std::endl;  //Voice�{�����[��
    //����
    file.close();
}
