#include "LoadingManager.h"
#include "game.h"
#include "Math/MyMath.h"
#include <DxLib.h>

namespace
{
	//�ŒჍ�[�h����
	constexpr int kMinLoadFrame = 60;
}

void  LoadingManager::Init()
{
	//���[�h��
	m_loadingNum = GetASyncLoadNum();
	//������
	m_countFrame = 0;
}
void  LoadingManager::Update()
{
	m_countFrame++;
}
void  LoadingManager::Draw()const
{
	if (!IsLoading())return;
	if (m_loadingNum <= 0)return;
	DrawFormatString(Game::kScreenCenterX, Game::kScreenCenterY,
		0xffffff, L"%d%%", 100 * (1 - GetASyncLoadNum() / m_loadingNum));
}
void  LoadingManager::End()
{
	//�񓯊����[�h�I��
	StopLoading();
}

void LoadingManager::StartLoading()
{
	//�񓯊����[�h�J�n
	SetUseASyncLoadFlag(true);
}

void LoadingManager::StopLoading()
{
	//�񓯊����[�h�I��
	SetUseASyncLoadFlag(false);
}

bool LoadingManager::IsLoading() const
{
	return (GetASyncLoadNum() > 0) || (m_countFrame <= kMinLoadFrame);
}
