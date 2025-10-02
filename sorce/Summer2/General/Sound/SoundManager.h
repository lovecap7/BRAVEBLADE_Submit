#pragma once
#include <map>
#include <string>
#include <list>
#include <memory>
class SoundBase;
class BGM;
class SE;
class Voice;
/// <summary>
/// ��(BGM,SE)���Ǘ�����V���O���g���N���X
/// </summary>
class SoundManager final
{
private:
	//�V���O���g���̏���
	SoundManager() = default;
	~SoundManager() = default;
	//�R�s�[�֎~
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator = (const SoundManager&) = delete;
	//���[�u�֎~
	SoundManager(SoundManager&&) = delete;
	SoundManager& operator = (SoundManager&&) = delete;
public:
	//�C���X�^���X���擾
	static SoundManager& GetInstance()
	{
		static SoundManager instance;
		return instance;
	}
	//�o�^
	void Entry(std::shared_ptr<SoundBase> sound);
	//�o�^����
	void Exit(std::shared_ptr<SoundBase> sound);
	//������
	void Init();
	//�X�V
	void Update();
	//�I��
	void End();
	//�ۑ�
	void SaveVolume();
	//BGM�Đ�
	void PlayBGM(std::string name);
	//BGM��~
	void StopBGM();
	//1�x�����Đ����� 
	std::weak_ptr<SE> PlayOnceSE(std::string name);
	std::weak_ptr<SE> PlayOnceSE(int handle);
	//���[�v�Đ�����
	std::weak_ptr<SE> PlayLoopSE(std::string name);
	std::weak_ptr<SE> PlayLoopSE(int handle);
	//1�x�����Đ����� 
	std::weak_ptr<Voice> PlayVC(std::string name);
	std::weak_ptr<Voice> PlayVC(int handle);
	//�S�Ă̍Đ����J�n
	void AllPlay();
	//�S�Ă̍Đ����~�߂�
	void AllStop();
	//����
	int GetSEVolumeC()	 const;
	int GetBGMVolumeC()	 const;
	int GetVoiceVolumeC() const;
	int GetMasterVolume()const;
	//�Z�b�^�[
	void SetSEVolume(int volume);
	void SetBGMVolume(int volume);
	void SetVoiceVolume(int volume);
	void SetMasterVolume(int volume);
	//���ʒ����p
	int GetSEVolume()	 const { return m_seVolume; };
	int GetBGMVolume()	 const { return m_bgmVolume; };
	int GetVoiceVolume() const { return m_voiceVolume; };
private:
	//SE�̉���
	int m_seVolume;
	//BGM�̉���
	int m_bgmVolume;
	//�{�C�X�̉���
	int m_voiceVolume;
	//�}�X�^�[�̉���
	int m_masterVolume;
	//�n���h���̔z��
	std::map<std::string, int> m_soundHandles;
	//�T�E���h
	std::list<std::shared_ptr<SoundBase>> m_sounds;
	//BGM�͏�Ɉ�Ȃ̂ł����ň���
	std::shared_ptr<BGM> m_bgm;
	//�폜���̂��̂��폜
	void CheckDeleteSound();
};
