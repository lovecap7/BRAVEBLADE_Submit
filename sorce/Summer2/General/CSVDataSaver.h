#pragma once
#include "CSVDatas.h"
#include <string>
#include <memory>
class Score;
/// <summary>
/// �ۑ��������f�[�^��CSV�ɕۑ�����
/// </summary>
class CSVDataSaver
{
public:
	CSVDataSaver();
	~CSVDataSaver();
	/// <summary>
	/// �X�R�A��ۑ�����
	/// </summary>
	/// <param name="score"></param>
	void SaveScoreDataToCSV(std::shared_ptr<Score> score);

	/// <summary>
	/// �Q�[���̐i���x�Ȃǂ��L�^
	/// </summary>
	void SaveDataToCSV(SaveDatas saveDatas);
	/// <summary>
	/// ���ʂ�ۑ�
	/// </summary>
	void SaveVolumeDataToCSV(VolumeData volumeDatas);
};

