#include "PurpleDinosaurStateIdle.h"
#include "PurpleDinosaurStateDeath.h"
#include "PurpleDinosaurStateChase.h"
#include "PurpleDinosaurStateAttack.h"
#include "PurpleDinosaurStateHit.h"
#include "PurpleDinosaurStateSearch.h"
#include "PurpleDinosaur.h"
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
	//�v���C���[�ɍU�����鋗��
	constexpr float kAttackDistance = 200.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V�����̖��O
	const char* kAnim = "CharacterArmature|Idle";//�ҋ@
	//�x���t���[��(�^�[�Q�b�g���������Ă����t���[���o�߂���܂ł͌x��)
	constexpr int kWarningFrame = 120;
}

PurpleDinosaurStateIdle::PurpleDinosaurStateIdle(std::weak_ptr<Actor> owner):
	PurpleDinosaurStateBase(owner),
	m_warningFrame(kWarningFrame)
{
	//�ҋ@���
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Normal);
}

PurpleDinosaurStateIdle::~PurpleDinosaurStateIdle()
{
}

void PurpleDinosaurStateIdle::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateIdle::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�R���C�_�u��
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	//���S
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<PurpleDinosaurStateDeath>(m_owner));
		return;
	}
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<PurpleDinosaurStateHit>(m_owner));
		return;
	}

	auto targetData = coll->GetTargetData();
	//�v���C���[���������Ȃ�
	if (targetData.isHitTarget)
	{
		//�v���C���[������
		coll->LookAtTarget();
		//�x��
		m_warningFrame = kWarningFrame;
		coll->SetIsWarning(true);

		//�U���̋���
		if (targetData.targetDis <= kAttackDistance)
		{
			//�U���̃N�[���^�C����0�Ȃ�
			if (coll->GetAttackCoolTime() <= 0 && coll->CanAttack())
			{
				//�U����Ԃɂ���
				ChangeState(std::make_shared<PurpleDinosaurStateAttack>(m_owner,actorManager));
				return;
			}
		}
		//�U���̋�������Ȃ��Ƃ��͋߂Â�
		else
		{
			//�v���C���[���ɋ߂Â�
			ChangeState(std::make_shared<PurpleDinosaurStateChase>(m_owner));
			return;
		}
	}
	//�T���ꏊ������Ȃ�
	else if (coll->IsHaveSearchPlace())
	{
		--m_warningFrame;
		if (m_warningFrame <= 0)
		{
			//���G��Ԃɂ���
			ChangeState(std::make_shared<PurpleDinosaurStateSearch>(m_owner));
			return;
		}
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}