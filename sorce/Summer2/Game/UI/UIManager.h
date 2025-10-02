#pragma once
#include <memory>
#include <list>
#include <map>
#include <string>
class UIBase;
class Player;
class EnemyBase;
class Score;
class Timer;
class BossHPUI;
class GameScoreUI;
class TimerUI;
class UIManager final
{
private:
	//�V���O���g���̏���
	UIManager() = default;
	~UIManager() = default;
	//�R�s�[�֎~
	UIManager(const UIManager&) = delete;
	UIManager& operator = (const UIManager&) = delete;
	//���[�u�֎~
	UIManager(UIManager&&) = delete;
	UIManager& operator = (UIManager&&) = delete;
public:
	//�C���X�^���X���擾
	static UIManager& GetInstance()
	{
		static UIManager instance;
		return instance;
	}
	//�o�^
	void Entry(std::shared_ptr<UIBase> ui);
	//����
	void Exit(std::shared_ptr<UIBase> ui);
	void Init();
	void Update();
	//���`��
	void BackDraw()const;
	//�O�`��
	void FrontDraw()const;
	void End();
	void Reset();

	//�v���C���[�֘A��UI
	void CreatePlayerUI(const std::weak_ptr<Player> player);
	//�{�XUI
	std::weak_ptr<BossHPUI> CreateBossUI(const std::weak_ptr<EnemyBase> boss);
	//�GUI
	void CreateEnemyUI(const std::weak_ptr<EnemyBase> enemy);
	//�X�R�A
	std::weak_ptr<GameScoreUI> CreateGameScoreUI(const std::weak_ptr<Score> score);
	//�^�C�}�[
	std::weak_ptr<TimerUI> CreateTimerUI(const std::weak_ptr<Timer> timer);
	//�摜�n���h���擾
	int GetImageHandle(const std::string& name)const;
	//�e�L�X�g�n���h���擾
	int GetTextHandle(const std::string& name)const;
	//�n���h�����[�h
	void LoadHandle();
	//UI�̃n���h���폜
	void AllDeleteHandle();
	//�X�V���~�߂�
	void StopUpdate() { m_isUpdate = false; };
	//�X�V������
	void StartUpdate() { m_isUpdate = true; };
	//�`����~�߂�
	void StopDraw() { m_isDraw = false; };
	//�`�������
	void StartDraw() { m_isDraw = true; };
	//�e��UI�̕`����~�߂�
	void AllStopDraw();
	//�e��UI�̕`�������
	void AllStartDraw();
private:
	//UI
	std::list<std::shared_ptr<UIBase>> m_uis;
	//UI�̍폜
	void AllDeleteUIs();
	//�폜�\���UI���폜
	void CheckDelete();
	//�摜�n���h��
	std::map<std::string, int> m_imageHandles;
	//�e�L�X�g�n���h��
	std::map<std::string, int> m_textHandles;
	//�X�V������
	bool m_isUpdate;
	//�`�悷��
	bool m_isDraw;
};

