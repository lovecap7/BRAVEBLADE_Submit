#include "BGM.h"
#include <DxLib.h>
#include "SoundManager.h"

BGM::BGM(int handle, int volume):
	SoundBase(handle,volume,true)
{
}

BGM::~BGM()
{
}
void BGM::Update()
{
	SetVolume(SoundManager::GetInstance().GetBGMVolumeC());
}

void BGM::ChangeBGM(int handle)
{
	//�Đ����̃n���h��������Ȃ�
	if (m_playHandle >= 0)
	{
		//�Đ����~�߂�
		Stop();
		//�폜
		DeleteSoundMem(m_playHandle);
	}
	//�Đ�
	Play();
}
