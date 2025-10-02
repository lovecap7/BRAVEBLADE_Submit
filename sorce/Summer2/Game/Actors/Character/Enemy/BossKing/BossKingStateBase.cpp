#include "BossKingStateBase.h"
#include "BossKingStateMagicWave.h"
#include "BossKingStateBlastAttack.h"
#include "BossKingStateBeam.h"
#include "BossKingStateBack.h"
#include "BossKingStateRandMove.h"
#include "BossKingStateRapidFire.h"
#include "BossKingStateImpact.h"
#include "BossKingStateUlt.h"
#include "BossKing.h"

BossKingStateBase::BossKingStateBase(std::weak_ptr<Actor> owner, bool isTransformSecond):
	CharacterStateBase(owner),
	m_isTransformSecond(isTransformSecond)
{
}

BossKingStateBase::~BossKingStateBase()
{
}


void BossKingStateBase::ThinkAttack(const std::weak_ptr<ActorManager> actorManager)
{
	if (m_owner.expired())return;			//Š—LÒ‚ª‚¢‚È‚¢‚È‚ç
	if (!m_owner.lock()->CanAttack())return;//UŒ‚Œ ‚ª‚È‚¢‚È‚ç

	//‹——£‚ğŒ©‚ÄUŒ‚‚ğŒˆ‚ß‚é
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	auto targetData = coll->GetTargetData();
	if (!m_isTransformSecond)
	{
		ThinkAttackFirst(targetData, actorManager);
	}
	else
	{
		ThinkAttackSecond(targetData, actorManager);
	}
}
void BossKingStateBase::RandAttackVC()
{
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	switch (MyMath::GetRand(1, 5))
	{
	case 1:
		coll->CharacterVC("Attack1");
		break;
	case 2:
		coll->CharacterVC("Attack2");
		break;
	case 3:
		coll->CharacterVC("Attack3");
		break;
	case 4:
		coll->CharacterVC("Attack4");
		break;
	case 5:
		coll->CharacterVC("Attack5");
		break;
	default:
		break;
	}
}

void BossKingStateBase::ThinkAttackFirst(CharacterBase::TargetData& targetData, const std::weak_ptr<ActorManager> actorManager)
{
	//‹ß‹——£‚É‚¢‚é‚È‚ç
	if (targetData.targetDis <= kMeleeAttackDistance)
	{
		//UŒ‚‚Ì”
		int attackNum = 3;
		//ƒ‰ƒ“ƒ_ƒ€‚ÉŒˆ’è
		auto rand = MyMath::GetRand(1, attackNum);
		switch (rand)
		{
		case 1:
			ChangeState(std::make_shared<BossKingStateMagicWave>(m_owner, actorManager));
			break;
		case 2:
			ChangeState(std::make_shared<BossKingStateBlastAttack>(m_owner, actorManager));
			break;
		case 3:
			ChangeState(std::make_shared<BossKingStateBack>(m_owner));
			break;
		}
		return;
	}
	//‰“‹——£‚È‚ç
	else
	{
		//UŒ‚‚Ì”
		int attackNum = 3;
		//ƒ‰ƒ“ƒ_ƒ€‚ÉŒˆ’è
		auto rand = MyMath::GetRand(1, attackNum);
		switch (rand)
		{
		case 1:
			ChangeState(std::make_shared<BossKingStateMagicWave>(m_owner, actorManager));
			break;
		case 2:
			ChangeState(std::make_shared<BossKingStateBeam>(m_owner, m_isTransformSecond,actorManager));
			break;
		case 3:
			ChangeState(std::make_shared<BossKingStateBlastAttack>(m_owner, actorManager));
			break;
		}
		return;
	}
}

void BossKingStateBase::ThinkAttackSecond(CharacterBase::TargetData& targetData, const std::weak_ptr<ActorManager> actorManager)
{
	//‹ß‹——£‚É‚¢‚é‚È‚ç
	if (targetData.targetDis <= kMeleeAttackDistance)
	{
		//UŒ‚‚Ì”
		int attackNum = 5;
		//ƒ‰ƒ“ƒ_ƒ€‚ÉŒˆ’è
		auto rand = MyMath::GetRand(1, attackNum);
		switch (rand)
		{
		case 1:
			ChangeState(std::make_shared<BossKingStateRandMove>(m_owner, m_isTransformSecond));
			break;
		case 2:
			ChangeState(std::make_shared<BossKingStateBeam>(m_owner, m_isTransformSecond,actorManager));
			break;
		case 3:
			ChangeState(std::make_shared<BossKingStateRapidFire>(m_owner, actorManager));
			break;
		case 4:
			ChangeState(std::make_shared<BossKingStateImpact>(m_owner, actorManager));
			break;
		case 5:
			ChangeState(std::make_shared<BossKingStateUlt>(m_owner, actorManager));
			break;
		}
		return;
	}
	//‰“‹——£‚È‚ç
	else
	{
		//UŒ‚‚Ì”
		int attackNum = 4;
		//ƒ‰ƒ“ƒ_ƒ€‚ÉŒˆ’è
		auto rand = MyMath::GetRand(1, attackNum);
		switch (rand)
		{
		case 1:
			ChangeState(std::make_shared<BossKingStateRandMove>(m_owner, m_isTransformSecond));
			break;
		case 2:
			ChangeState(std::make_shared<BossKingStateBeam>(m_owner, m_isTransformSecond, actorManager));
			break;
		case 3:
			ChangeState(std::make_shared<BossKingStateRapidFire>(m_owner, actorManager));
			break;
		case 4:
			ChangeState(std::make_shared<BossKingStateUlt>(m_owner, actorManager));
			break;
		}
		return;
	}
}
