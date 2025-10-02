#include "LoadingManager.h"
#include "game.h"
#include "Math/MyMath.h"
#include <DxLib.h>

namespace
{
	//最低ロード時間
	constexpr int kMinLoadFrame = 60;
}

void  LoadingManager::Init()
{
	//ロード数
	m_loadingNum = GetASyncLoadNum();
	//初期化
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
	//非同期ロード終了
	StopLoading();
}

void LoadingManager::StartLoading()
{
	//非同期ロード開始
	SetUseASyncLoadFlag(true);
}

void LoadingManager::StopLoading()
{
	//非同期ロード終了
	SetUseASyncLoadFlag(false);
}

bool LoadingManager::IsLoading() const
{
	return (GetASyncLoadNum() > 0) || (m_countFrame <= kMinLoadFrame);
}
