#include "BomberStateIdle.h"
#include "BomberStateHit.h"
#include "BomberStateChase.h"
#include "BomberStateDeath.h"
#include "BomberStateAttack.h"
#include "BomberStateSearch.h"
#include "Bomber.h"
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
	//�v���C���[�퓬��ԂɂȂ鋗��
	constexpr float kAttackDistance = 400.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V�����̖��O
	const char* kAnim = "CharacterArmature|Idle";//�ҋ@
	//�x���t���[��(�^�[�Q�b�g���������Ă����t���[���o�߂���܂ł͌x��)
	constexpr int kWarningFrame = 120;
}

BomberStateIdle::BomberStateIdle(std::weak_ptr<Actor> owner) :
	BomberStateBase(owner),
	m_warningFrame(kWarningFrame)
{
	//�ҋ@���
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Normal);
}

BomberStateIdle::~BomberStateIdle()
{
}

void BomberStateIdle::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BomberStateIdle::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�R���C�_�u��
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	//���S
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BomberStateDeath>(m_owner));
		return;
	}
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BomberStateHit>(m_owner));
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
				ChangeState(std::make_shared<BomberStateAttack>(m_owner, actorManager));
				return;
			}
		}
		//�U���̋�������Ȃ��Ƃ��͋߂Â�
		else
		{
			//�v���C���[���ɋ߂Â�
			ChangeState(std::make_shared<BomberStateChase>(m_owner));
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
			ChangeState(std::make_shared<BomberStateSearch>(m_owner));
			return;
		}
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}