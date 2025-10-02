#include "UltGage.h"
#include "../../../../General/Math/MathSub.h"

UltGage::UltGage(int max):
	m_ultGageValue(0),
	m_maxUltGageValue(max),
	m_pendingUltGauge(0)
{
}

UltGage::~UltGage()
{
}

void UltGage::AddUltGage(int add)
{
	//Å‘å‚È‚ç
	if (IsMaxUlt())return;
	//‰ÁZ
	m_ultGageValue += add;
	//ƒNƒ‰ƒ“ƒv
	m_ultGageValue = MathSub::ClampInt(m_ultGageValue, 0, m_maxUltGageValue);
}
