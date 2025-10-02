#include "SoundBase.h"
#include "SoundManager.h"
#include <DxLib.h>

SoundBase::SoundBase(int handle, int volume, bool isLoop):
	m_playHandle(handle),
	m_volume(volume),
	m_isLoop(isLoop)
{
	SetVolume(m_volume);
}

SoundBase::~SoundBase()
{
}

void SoundBase::Init()
{
	SoundManager::GetInstance().Entry(shared_from_this());
}
void SoundBase::End()
{
	Delete();
	SoundManager::GetInstance().Exit(shared_from_this());
}
void SoundBase::Play()
{
	SetVolume(m_volume);
	//�Đ�
	if (m_isLoop)	
	{
		//���[�v
		PlaySoundMem(m_playHandle, DX_PLAYTYPE_LOOP, true);
	}
	else
	{
		PlaySoundMem(m_playHandle, DX_PLAYTYPE_BACK, true);
	}
}
void SoundBase::Stop()
{
	//�Đ����~�߂�
	StopSoundMem(m_playHandle);
}
void SoundBase::Delete()
{
	//�폜
	DeleteSoundMem(m_playHandle);
}

bool SoundBase::IsPlay()
{
	return CheckSoundMem(m_playHandle);
}

bool SoundBase::IsDelete()
{
	//���[�v�Đ�����Ȃ��Đ����I�������Ƃ��폜����
	return !m_isLoop && IsPlay();
}

void SoundBase::SetVolume(int volume)
{
	m_volume = volume;
	ChangeVolumeSoundMem(m_volume, m_playHandle);
}
