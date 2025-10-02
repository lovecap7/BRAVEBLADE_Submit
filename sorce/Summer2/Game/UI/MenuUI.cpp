#include "MenuUI.h"
#include "UIManager.h"

namespace
{
	//選択中の座標をずらす値
	constexpr float kSelectOffsetPosX = 100.0f;
	//lerp率
	constexpr float kLerpRate = 0.1f;
	//通常サイズ
	constexpr float kNormalSize = 1.0f;
	//選択している間のサイズ
	constexpr float kSelectSize = 0.1f;
	//拡大速度
	constexpr float kSizeSpeed = 0.05f;
}

MenuUI::MenuUI(Vector2 pos, int handle) :
	UIBase(true),
	m_pos(pos),
	m_handle(handle),
	m_isSelect(false),
	m_isWait(false),
	m_size(kNormalSize),
	m_sizeAngle(0)
{
	
}

MenuUI::~MenuUI()
{

}


void MenuUI::Update()
{
	//待機状態
	if (m_isWait)
	{
		m_size = MathSub::Lerp(m_size, kNormalSize + kSelectSize, kLerpRate);
		return;
	}
	//選択中は拡大縮小
	if (m_isSelect)
	{
		m_sizeAngle += kSizeSpeed;
		if (m_sizeAngle > MyMath::TwoPI_F)m_sizeAngle -= MyMath::TwoPI_F;
		m_size = kNormalSize + kSelectSize * std::abs(sinf(m_sizeAngle));
	}
	else
	{
		m_size = MathSub::Lerp(m_size, kNormalSize, kLerpRate);
	}
}

void MenuUI::Draw() const
{
	if (!m_isDraw)return;
	//描画
	DrawRotaGraph(m_pos.x, m_pos.y, static_cast<double>(m_size), 0.0, m_handle, true);
}
