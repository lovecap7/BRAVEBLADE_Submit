#include "BossDragonStateBase.h"
#include "BossDragon.h"
#include "BossDragonStateChase.h"
#include "BossDragonStateBreathAttack.h"
#include "BossDragonStateSweepAttack.h"
BossDragonStateBase::BossDragonStateBase(std::weak_ptr<Actor> owner):
	CharacterStateBase(owner)
{
}

BossDragonStateBase::~BossDragonStateBase()
{
}


void BossDragonStateBase::ThinkAttack(const std::weak_ptr<ActorManager> actorManager)
{
	if (m_owner.expired())return;			//所有者がいないなら
	if (!m_owner.lock()->CanAttack())return;//攻撃権がないなら

	//ランダムに決定
	auto rand = MyMath::GetRand(0, 2);

	switch (rand)
	{
	case 0:
		//プレイヤーをに近づく
		ChangeState(std::make_shared<BossDragonStateChase>(m_owner));
		break;
	case 1:
		//薙ぎ払い
		ChangeState(std::make_shared<BossDragonStateSweepAttack>(m_owner, actorManager));
		break;
	case 2:
		//ブレス
		ChangeState(std::make_shared<BossDragonStateBreathAttack>(m_owner, actorManager));
		break;
	default:
		//ブレス
		ChangeState(std::make_shared<BossDragonStateBreathAttack>(m_owner, actorManager));
		break;
	}
	return;
}
