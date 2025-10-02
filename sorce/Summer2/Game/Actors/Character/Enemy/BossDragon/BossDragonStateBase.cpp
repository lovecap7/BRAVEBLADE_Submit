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
	if (m_owner.expired())return;			//���L�҂����Ȃ��Ȃ�
	if (!m_owner.lock()->CanAttack())return;//�U�������Ȃ��Ȃ�

	//�����_���Ɍ���
	auto rand = MyMath::GetRand(0, 2);

	switch (rand)
	{
	case 0:
		//�v���C���[���ɋ߂Â�
		ChangeState(std::make_shared<BossDragonStateChase>(m_owner));
		break;
	case 1:
		//�ガ����
		ChangeState(std::make_shared<BossDragonStateSweepAttack>(m_owner, actorManager));
		break;
	case 2:
		//�u���X
		ChangeState(std::make_shared<BossDragonStateBreathAttack>(m_owner, actorManager));
		break;
	default:
		//�u���X
		ChangeState(std::make_shared<BossDragonStateBreathAttack>(m_owner, actorManager));
		break;
	}
	return;
}
