#pragma once
#include <map>
#include <memory>
#include <string>
#include "../General/StageIndex.h"
class Score;
//�Q�[���̐i���x�ȂǋL�^���ׂ����̂��L�^��
//�Q�[���̊J�n���Ƀ��[�h���A�Q�[���I�����ɋL�^������V���O���g���N���X
class SaveDataManager final
{
private:
	//�V���O���g���̏���
	SaveDataManager() = default;
	~SaveDataManager() = default;
	//�R�s�[�֎~
	SaveDataManager(const SaveDataManager&) = delete;
	SaveDataManager& operator = (const SaveDataManager&) = delete;
	//���[�u�֎~
	SaveDataManager(SaveDataManager&&) = delete;
	SaveDataManager& operator = (SaveDataManager&&) = delete;
public:
	//�C���X�^���X���擾
	static SaveDataManager& GetInstance()
	{
		static SaveDataManager instance;
		return instance;
	}
	//������
	void Init();
	//�X�V
	void Update();
	//�I������
	void End();

	//���[�h
	void Load();
	//�L�^
	void Save();
	//�f�[�^�����������čŏ�����
	void NewGame();

	//�N���A�����X�e�[�W���L�^
	void SaveClearStage(Stage::StageIndex stageIndex);
	//���̃X�e�[�W���N���A�����̂�
	bool IsClearStage(Stage::StageIndex stageIndex);
	//���v���C����
	unsigned int GetTotalPlayTime()const { return m_totalPlayTime; };
	//�X�R�A
	std::weak_ptr<Score> GetScore() { return m_score; };
	//���t�Ǝ���
	std::wstring GetLastSaveTimeAndDate() { return m_lastSaveTime; };
	//�X�e�[�W�S�J��
	void AllClearStage();
private:
	//3�X�e�[�W���̏���N���A�L�^������(�X�e�[�W�̐i���x�Ɏg��)
	std::map<Stage::StageIndex, bool>m_isClearStage;
	//�v���C����
	unsigned int m_totalPlayTime;
	//�X�R�A
	std::shared_ptr<Score> m_score;
	//���t�Ǝ���
	std::wstring m_lastSaveTime;
private:
	//���t�Ǝ���
	std::wstring GetNowTimeAndDate();
};

