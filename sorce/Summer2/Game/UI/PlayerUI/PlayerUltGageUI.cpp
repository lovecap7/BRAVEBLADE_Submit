#include "PlayerUltGageUI.h"
#include "../../Actors/Character/Player/UltGage.h"
#include "../../Actors/Character/Player/Player.h"
#include "../../Actors/ActorManager.h"
#include "../UIManager.h"
#include <DxLib.h>
namespace
{
	constexpr float kBarHeight = 50.0f;
	constexpr float kBarWidth = 400.0f;
	constexpr float kLeftPosX = 50.0f;
	constexpr float kLeftPosY = 110.0f;
	constexpr float kRightPosY = kLeftPosY + kBarHeight;
	//拡大縮小の速度
	constexpr float kScaleSpeed = 1.0f / 10.0f;
	//拡大倍率
	constexpr float kMaxScaleRate = 0.1f;
	//ゲージボタンの位置
	constexpr float kBottunPosX = kLeftPosX + kBarWidth;
	constexpr float kBottunPosY = 160;
	//ゲージが最大の時の光の倍率
	constexpr float kMaxUltGageBrightRate = 100.0f;
}

PlayerUltGageUI::PlayerUltGageUI(std::weak_ptr<Player> player):
	PlayerUIBase(player),
	m_viewUltGageValue(0.0f),
	m_viewMaxUltGageValue(0.0f),
	m_ultGageFrameHandle(UIManager::GetInstance().GetImageHandle("PlayerGageFrame")),
	m_ultGageHandle(UIManager::GetInstance().GetImageHandle("PlayerUltGage")),
	m_maxUltGageHandle(UIManager::GetInstance().GetImageHandle("PlayerMaxUltGage")),
	m_ultBottunHandle(UIManager::GetInstance().GetImageHandle("UltBottun")),
	m_bottunScale(1.0),
	m_bottunAngle(0.0f)
{
	//プレイヤーが消えた場合このUIも削除
	if (m_player.expired())
	{
		m_isDelete = true;
		return;
	}
	auto ultGage = m_player.lock()->GetUltGage().lock();
	m_viewUltGageValue = ultGage->GetUltGageValue();
	m_viewMaxUltGageValue = ultGage->GetMaxUltGageValue();
}

PlayerUltGageUI::~PlayerUltGageUI()
{
}

void PlayerUltGageUI::Update()
{
	if (!m_isDraw)return;
	//プレイヤーが消えた場合はこのUIも削除
	if (m_player.expired())
	{
		m_isDelete = true;
		return;
	}
	auto player = m_player.lock();
	//プレイヤーがゲーム開始アニメーション中かクリアアニメーション中なら描画しない
	if (player->IsStartAnim() || player->IsClearAnim())
	{
		m_isDraw = false;
	}
	else
	{
		m_isDraw = true;
	}
	auto ultGage = player->GetUltGage().lock();
	//更新
	//体力に変動があった時
	if (m_viewUltGageValue != ultGage->GetUltGageValue())
	{
		m_viewUltGageValue = ultGage->GetUltGageValue();
	}
	if (m_viewMaxUltGageValue != ultGage->GetMaxUltGageValue())
	{
		m_viewMaxUltGageValue = ultGage->GetMaxUltGageValue();
	}
	//ボタンの拡大縮小
	if (m_viewUltGageValue >= m_viewMaxUltGageValue)
	{
		m_bottunAngle += kScaleSpeed;
		if (m_bottunAngle > MyMath::TwoPI_F)m_bottunAngle -= MyMath::TwoPI_F;
		m_bottunScale = 1.0f + kMaxScaleRate * sinf(m_bottunAngle);
	}
	else
	{
		m_bottunAngle = 0.0f;
		m_bottunScale = 1.0;
	}
}

void PlayerUltGageUI::Draw() const
{
	//描画しないならreturn
	if (!m_isDraw)return;
	DrawBoxAA(kLeftPosX, kLeftPosY, kLeftPosX + (m_viewMaxUltGageValue / m_viewMaxUltGageValue) * kBarWidth, kRightPosY, 0x555555, true);
	//ゲージ
	DrawRectGraph(kLeftPosX, kLeftPosY, 0, 0,
		kBarWidth * (m_viewUltGageValue / m_viewMaxUltGageValue), kBarHeight, m_ultGageHandle, true);
	//ゲージが最大なら光らせる
	if (m_viewUltGageValue >= m_viewMaxUltGageValue)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, m_bottunAngle * kMaxUltGageBrightRate);
		DrawRectGraph(kLeftPosX, kLeftPosY, 0, 0,
			kBarWidth * (m_viewUltGageValue / m_viewMaxUltGageValue), kBarHeight, m_maxUltGageHandle, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	//フレーム
	DrawGraph(kLeftPosX, kLeftPosY, m_ultGageFrameHandle, true);
	//ゲージが最大ならボタンを表示
	if (m_viewUltGageValue >= m_viewMaxUltGageValue)
	{
		DrawRotaGraph(kBottunPosX, kBottunPosY, m_bottunScale, 0.0, m_ultBottunHandle, true);
	}
}

