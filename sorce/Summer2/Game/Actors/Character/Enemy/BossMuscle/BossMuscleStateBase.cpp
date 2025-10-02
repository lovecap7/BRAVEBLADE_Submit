#include "BossMuscleStateBase.h"
#include "BossMuscleStateRightPunch.h"
#include "BossMuscleStateBeam.h"
#include "BossMuscleStateJumpAttack.h"
#include "BossMuscleStateRandMove.h"
#include "BossMuscleStateSwingAttack.h"
#include "BossMuscle.h"

BossMuscleStateBase::BossMuscleStateBase(std::weak_ptr<Actor> owner, bool isAngry) :
	CharacterStateBase(owner),
	m_isAngry(isAngry)
{
}

BossMuscleStateBase::~BossMuscleStateBase()
{
}


void BossMuscleStateBase::ThinkAttack(const std::weak_ptr<ActorManager> actorManager)
{
	if (m_owner.expired())return;			//Š—LÒ‚ª‚¢‚È‚¢‚È‚ç
	if (!m_owner.lock()->CanAttack())return;//UŒ‚Œ ‚ª‚È‚¢‚È‚ç

	//‹——£‚ğŒ©‚ÄUŒ‚‚ğŒˆ‚ß‚é
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	auto targetData = coll->GetTargetData();
	//‹ß‹——£‚É‚¢‚é‚È‚ç
	if (targetData.targetDis <= kMeleeAttackDistance)
	{
		//UŒ‚‚Ì”
		int attackNum = 3;
		if (m_isAngry)attackNum += 1; //“{‚èó‘Ô‚È‚çUŒ‚‚Ìí—Ş‚ğ‘‚â‚·
		//ƒ‰ƒ“ƒ_ƒ€‚ÉŒˆ’è
		auto rand = MyMath::GetRand(1, attackNum);
		switch (rand)
		{
		case 1:
			ChangeState(std::make_shared<BossMuscleStateRightPunch>(m_owner, m_isAngry, actorManager));
			break;
		case 2:
			ChangeState(std::make_shared<BossMuscleStateJumpAttack>(m_owner, m_isAngry, actorManager));
			break;
		case 3:
			ChangeState(std::make_shared<BossMuscleStateRandMove>(m_owner, m_isAngry));
			break;
		case 4:
			ChangeState(std::make_shared<BossMuscleStateSwingAttack>(m_owner, m_isAngry, actorManager));
			break;
		}
		return;
	}
	//‰“‹——£‚È‚ç
	else
	{
		//UŒ‚‚Ì”
		int attackNum = 3;
		if (m_isAngry)attackNum += 1; //“{‚èó‘Ô‚È‚çUŒ‚‚Ìí—Ş‚ğ‘‚â‚·
		//ƒ‰ƒ“ƒ_ƒ€‚ÉŒˆ’è
		auto rand = MyMath::GetRand(1, attackNum);
		switch (rand)
		{
		case 1:
			ChangeState(std::make_shared<BossMuscleStateBeam>(m_owner, m_isAngry, actorManager));
			break;
		case 2:
			ChangeState(std::make_shared<BossMuscleStateJumpAttack>(m_owner, m_isAngry, actorManager));
			break;
		case 3:
			ChangeState(std::make_shared<BossMuscleStateRandMove>(m_owner, m_isAngry));
			break;
		case 4:
			ChangeState(std::make_shared<BossMuscleStateSwingAttack>(m_owner, m_isAngry, actorManager));
			break;
		}
		return;
	}
}
