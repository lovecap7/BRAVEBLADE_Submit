#include "LeftArrowUI.h"
#include "UIManager.h"

LeftArrowUI::LeftArrowUI(Vector2 pos,bool isRight):
	m_isSelect(false),
	m_scale(1.0),
	m_baseScale(1.0),
	m_scaleAngle(0.0f),
	m_arrowHandle(UIManager::GetInstance().GetImageHandle("LeftArrow")),
	m_lockArrowHandle(UIManager::GetInstance().GetImageHandle("LockLeftArrow")),
	m_pos(pos),
	m_isRight(isRight),
	m_isLock(false)
{
}

LeftArrowUI::~LeftArrowUI()
{
}

void LeftArrowUI::Update()
{
	if (m_isSelect)
	{
		m_scaleAngle += 0.1;
		if (m_scaleAngle > MyMath::TwoPI_F)m_scaleAngle -= MyMath::TwoPI_F;
		m_scale = m_baseScale + 0.1 * sinf(m_scaleAngle);
	}
	else
	{
		m_scale = m_baseScale;
		m_scaleAngle = 0.0f;
	}
}

void LeftArrowUI::Draw() const
{
	if (!m_isDraw)return;
	int handle = m_arrowHandle;
	if (m_isLock)handle = m_lockArrowHandle;//Œ®‚ª‚©‚©‚Á‚Ä‚é‚È‚çŒ®•t‚«‚Ì–îˆó
	DrawRotaGraphF(m_pos.x, m_pos.y, m_scale, 0.0, handle, true, m_isRight);
}
