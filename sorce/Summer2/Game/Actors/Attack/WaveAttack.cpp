#include "WaveAttack.h"
#include "../../../General/Collision/TorusCollider.h"
#include "../../../General/Rigidbody.h"
#include <DxLib.h>
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Sound/SoundManager.h"
#include "../../../General/Math/MyMath.h"
WaveAttack::WaveAttack(std::weak_ptr<Actor> owner):
	TorusAttackBase(owner),
	m_waveSpeed(0.0f)
{
}

WaveAttack::~WaveAttack()
{
}

void WaveAttack::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	AttackBase::Update(actorManager);
	//広がる
	SetRange(std::dynamic_pointer_cast<TorusCollider>(m_collisionData)->GetRange() + m_waveSpeed);
}

void WaveAttack::Draw() const
{
#if _DEBUG
	auto coll = std::dynamic_pointer_cast<TorusCollider>(m_collisionData);
	DrawSphere3D(m_rb->m_pos.ToDxLibVector(),
		coll->GetRange(), 16, 0xff0000, 0xffffff, false);
	for(int i = 0; i < 20; ++i)
	{
		float angleDeg = i * (360.0f / 20.0f);
		float angleRad = MyMath::PI_F * angleDeg / 180.0f;
		// XZ平面の円周上に配置
		float x = cosf(angleRad) * coll->GetRange();
		float z = sinf(angleRad) * coll->GetRange();
		Vector3 offset = { x, 0.0f, z };
		Vector3 pos = m_rb->m_pos + offset;
		DrawSphere3D(pos.ToDxLibVector(), coll->GetRadius(), 16, 0xff0000, 0xffffff, false);
	}
#endif
}

void WaveAttack::OnCollide(const std::shared_ptr<Collidable> other)
{
	AttackBase::OnCollide(other);
	if (m_isHit)
	{
		//SE
		if (MyMath::IsRand())
		{
			SoundManager::GetInstance().PlayOnceSE("Hit1");
		}
		else
		{
			SoundManager::GetInstance().PlayOnceSE("Hit2");
		}
		//ヒットエフェクト
		auto coll = std::dynamic_pointer_cast<TorusCollider>(m_collisionData);
		auto hitPos = std::dynamic_pointer_cast<Actor>(other)->GetNextPos();
		auto eff = EffekseerManager::GetInstance().CreateEffect("ImpactHitEff", hitPos);
	}
}
