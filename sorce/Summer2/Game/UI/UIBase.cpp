#include "UIBase.h"
#include "UIManager.h"

UIBase::UIBase(bool isFront, Priority priority):
	m_isDelete(false),
	m_isDraw(true),
	m_isFront(isFront),
	m_priority(priority)
{
}

UIBase::~UIBase()
{
}

void UIBase::Init()
{
	//UIManager‚É“o˜^
	UIManager::GetInstance().Entry(shared_from_this());
}

void UIBase::End()
{
	//UIManager‚©‚ç“o˜^‰ğœ
	UIManager::GetInstance().Exit(shared_from_this());
}