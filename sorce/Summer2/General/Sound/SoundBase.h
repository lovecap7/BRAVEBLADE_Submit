#pragma once
#include <memory>
class SoundBase abstract:
	public std::enable_shared_from_this<SoundBase>
{
public:
	SoundBase(int handle,int volume,bool isLoop);
	~SoundBase();
	virtual void Init();
	virtual void Update()abstract;
	virtual void End();
	//�Đ�
	virtual void Play();
	//��~
	virtual void Stop();
	//�폜
	virtual void Delete();
	//�Đ����Ȃ�true
	virtual bool IsPlay();
	//�폜�\��
	virtual bool IsDelete();
	//����
	void SetVolume(int volume);
protected:
	//�n���h��
	int m_playHandle;
	//����
	int m_volume;
	//���[�v
	bool m_isLoop;
};

