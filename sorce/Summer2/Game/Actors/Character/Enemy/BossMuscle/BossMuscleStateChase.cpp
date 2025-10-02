#include "BossMuscleStateChase.h"
#include "BossMuscleStateIdle.h"
#include "BossMuscleStateDeath.h"
#include "BossMuscleStateHit.h"
#include "BossMuscle.h"
#include "../EnemyBase.h"
#include "../../../ActorManager.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
namespace
{
	//�A�j���[�V����
	const char* kAnim = "Boss|Walk";
	//�v���C���[��ǂ������鑬�x
	constexpr float kChaseSpeed = 2.0f;
}

BossMuscleStateChase::BossMuscleStateChase(std::weak_ptr<Actor> owner, bool isAngry):
	BossMuscleStateBase(owner, isAngry)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetCollState(CollisionState::Move);
	//�A�j���[�V����
	coll->GetModel()->SetAnim(kAnim, true);
}

BossMuscleStateChase::~BossMuscleStateChase()
{
}

void BossMuscleStateChase::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossMuscleStateChase::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�R���C�_�u��
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//���S
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BossMuscleStateDeath>(m_owner));
		return;
	}
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossMuscleStateHit>(m_owner,m_isAngry));
		return;
	}
	//�v���C���[
	auto player = actorManager.lock()->GetPlayer();

	auto targetData = coll->GetTargetData();
	//�v���C���[���������Ȃ�
	if (targetData.isHitTarget)
	{
		//�v���C���[������
		coll->LookAtTarget();
		//�U���̃N�[���^�C����0�Ȃ�
		if (coll->GetAttackCoolTime() <= 0)
		{
			ThinkAttack(actorManager);
			return;
		}
		//�߂Â�����
		if (targetData.targetDis > kMeleeAttackDistance)
		{
			//�v���C���[���ɋ߂Â�
			coll->GetRb()->SetMoveVec(targetData.targetDirXZ * kChaseSpeed);
			return;
		}
	}
	//�����܂ŗ�����
	//�ҋ@���
	ChangeState(std::make_shared<BossMuscleStateIdle>(m_owner, m_isAngry));
	return;
}