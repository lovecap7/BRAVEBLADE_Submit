#include "MenuUI.h"
#include "UIManager.h"

namespace
{
	//�I�𒆂̍��W�����炷�l
	constexpr float kSelectOffsetPosX = 100.0f;
	//lerp��
	constexpr float kLerpRate = 0.1f;
	//�ʏ�T�C�Y
	constexpr float kNormalSize = 1.0f;
	//�I�����Ă���Ԃ̃T�C�Y
	constexpr float kSelectSize = 0.1f;
	//�g�呬�x
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
	//�ҋ@���
	if (m_isWait)
	{
		m_size = MathSub::Lerp(m_size, kNormalSize + kSelectSize, kLerpRate);
		return;
	}
	//�I�𒆂͊g��k��
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
	//�`��
	DrawRotaGraph(m_pos.x, m_pos.y, static_cast<double>(m_size), 0.0, m_handle, true);
}
