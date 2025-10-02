#include "SearchPlace.h"
#include "../../General/Collision/SphereCollider.h"
#include "../../General/Rigidbody.h"
#include "Actor.h"
#include "../../Shader/MyDraw.h"
#include <DxLib.h>

SearchPlace::SearchPlace(Vector3 pos, float rang) :
	m_pos(pos),
	m_range(rang)
{
}

SearchPlace::~SearchPlace()
{
}

void SearchPlace::Draw() const
{
	MyDrawUtils::Draw3DCircle(m_pos, m_range,36,0xff0000);
}

bool SearchPlace::IsInSearchPlace(Vector3 targetPos) const
{
	//”ÍˆÍ“à‚É‚¢‚é‚©
	float dist = Vector3(targetPos - m_pos).Magnitude();
	if (dist <= m_range)
	{
		return true;
	}
	//‚±‚±‚Ü‚Å—ˆ‚½‚ç
	return false;
}
	