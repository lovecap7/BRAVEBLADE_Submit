#include "BossMuscleStateIdle.h"
#include "BossMuscleStateAngry.h"
#include "BossMuscleStateDeath.h"
#include "BossMuscleStateHit.h"
#include "BossMuscleStateRightPunch.h"
#include "BossMuscleStateBeam.h"
#include "BossMuscleStateJumpAttack.h"
#include "BossMuscleStateChase.h"
#include "BossMuscle.h"
#include "../EnemyBase.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../ActorManager.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"

namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V�����̖��O
	const char* kNormalAnim = "Boss|Idle1";//�ҋ@
	const char* kAnglyAnim = "Boss|Idle2";//�ҋ@
}

BossMuscleStateIdle::BossMuscleStateIdle(std::weak_ptr<Actor> owner, bool isAngry) :
	BossMuscleStateBase(owner,isAngry)
{
	//�ҋ@���
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	if (isAngry)
	{
		//�{���Ԃ̑ҋ@
		coll->GetModel()->SetAnim(kAnglyAnim, true);
	}
	else
	{
		//�ʏ�̑ҋ@
		coll->GetModel()->SetAnim(kNormalAnim, true);
	}
	coll->SetCollState(CollisionState::Normal);
}

BossMuscleStateIdle::~BossMuscleStateIdle()
{
}

void BossMuscleStateIdle::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossMuscleStateIdle::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
#if _DEBUG
	//�{�X��{�点��
	if (Input::GetInstance().IsTrigger("BossAngry"))
	{
		//�{���Ԃɂ���
		ChangeState(std::make_shared<BossMuscleStateAngry>(m_owner));
		return;
	}
#endif
	//�R���C�_�u��
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	auto targetData = coll->GetTargetData();
	//���S
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BossMuscleStateDeath>(m_owner));
		return;
	}
	//�̗͂������ȉ��Ȃ�
	if (coll->GetHitPoints().lock()->GetHp() <= coll->GetHitPoints().lock()->GetMaxHp() / 2 &&
		!m_isAngry)
	{
		//�{���Ԃɂ���
		ChangeState(std::make_shared<BossMuscleStateAngry>(m_owner));
		return;
	}
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossMuscleStateHit>(m_owner,m_isAngry));
		return;
	}
	//�v���C���[���������Ȃ�
	if (targetData.isHitTarget)
	{
		//�v���C���[������
		coll->LookAtTarget();
		//�߂Â�����
		if (targetData.targetDis > kMeleeAttackDistance)
		{
			//�v���C���[���ɋ߂Â�
			ChangeState(std::make_shared<BossMuscleStateChase>(m_owner, m_isAngry));
			return;
		}
		else
		{
			//�U���̃N�[���^�C����0�Ȃ�
			if (coll->GetAttackCoolTime() <= 0)
			{
				ThinkAttack(actorManager);
				return;
			}
		}
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
