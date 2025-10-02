#include "EnemyHPUI.h"
#include "../UIManager.h"
#include "../../../General/HitPoints.h"
#include "../../Actors/Character/Enemy/EnemyBase.h"
#include <DxLib.h>

namespace
{
	//描画位置の調整
	constexpr float kShiftLeftPosX = 70.0f;
	constexpr float kBarHeight = 20.0f;
	constexpr float kBarWidth = kShiftLeftPosX * 2.0f;
	constexpr float kBarOffsetPosY = 150.0f; //敵の頭の上に来るように調整
	//名前の位置
	constexpr float kNameOffsetPosY = kBarHeight + 20.0f;
	constexpr float kNameOffsetPosX = 10.0f;
}


EnemyHPUI::EnemyHPUI(std::weak_ptr<EnemyBase> enemy) :
	EnemyUIBase(enemy),
	m_viewHp(0.0f),
	m_viewMaxHp(0.0f),
	m_pos{},
	m_frameHandle(UIManager::GetInstance().GetImageHandle("EnemyHPFrame")),
	m_hpHandle(UIManager::GetInstance().GetImageHandle("EnemyHP")),
	m_textHandle(UIManager::GetInstance().GetTextHandle("源直ゴシック16")),
	m_name{}
{
	//敵が消えた場合このUIも削除
	if (m_enemy.expired())
	{
		m_isDelete = true;
		return;
	}
	auto owner = m_enemy.lock();
	auto hp = owner->GetHitPoints().lock();
	//初期化
	m_viewHp = hp->GetHp();
	m_viewMaxHp = hp->GetMaxHp();
}

EnemyHPUI::~EnemyHPUI()
{
}

void EnemyHPUI::Update()
{
	//敵が消えた場合このUIも削除
	if (m_enemy.expired())
	{
		m_isDelete = true;
		return;
	}
	auto enemy = m_enemy.lock();
	auto hp = enemy->GetHitPoints().lock();
	//更新
	//体力に変動があった時
	if (m_viewHp != hp->GetHp())
	{
		m_viewHp = hp->GetHp();
	}
	if (m_viewMaxHp != hp->GetMaxHp())
	{
		m_viewMaxHp = hp->GetMaxHp();
	}
	//名前
	if (m_name != enemy->GetName())
	{
		m_name = enemy->GetName();
	};

	//描画座標を更新
	m_pos = enemy->GetPos();
	//頭の上に来るぐらいに調整
	m_pos.y += kBarOffsetPosY;
}

void EnemyHPUI::Draw() const
{
	//描画しないならreturn
	if (!m_isDraw)return;
	auto pos = ConvWorldPosToScreenPos(m_pos.ToDxLibVector());
	pos.x -= kShiftLeftPosX;
	DrawBoxAA(pos.x, pos.y, pos.x + (m_viewMaxHp / m_viewMaxHp) * kBarWidth, pos.y - kBarHeight, 0x555555, true);
	//体力
	DrawRectGraph(pos.x, pos.y - kBarHeight, 0, 0,
		kBarWidth * (m_viewHp / m_viewMaxHp), kBarHeight, m_hpHandle, true);
	//フレーム
	DrawGraph(pos.x, pos.y - kBarHeight, m_frameHandle, true);
	//名前
	DrawStringToHandle(pos.x + kNameOffsetPosX, pos.y - kNameOffsetPosY, m_name.c_str(), 0xffffff, m_textHandle);
}
