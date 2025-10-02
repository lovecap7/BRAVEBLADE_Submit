#include "BossMuscleStateSwingAttack.h"
#include "BossMuscleStateIdle.h"
#include "BossMuscleStateDeath.h"
#include "BossMuscleStateHit.h"
#include "BossMuscle.h"
#include "../EnemyBase.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../ActorManager.h"
#include "../../../Attack/AreaOfEffectAttack.h"
namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�E��̃C���f�b�N�X
	constexpr int kRightHandIndex = 43;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 120;
	//�{���Ԃ̎��̑��x2�{
	constexpr float kAngryMoveSpeedRate = 2.0f;
	//�O�i����
	constexpr float kAttackMoveDistance = 200.0f;
}

BossMuscleStateSwingAttack::BossMuscleStateSwingAttack(std::weak_ptr<Actor> owner, bool isAngry, const std::weak_ptr<ActorManager> actorManager):
	BossMuscleStateBase(owner, isAngry),
	m_attackCountFrame(0)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kSwingAttackName);
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//�U��
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//����̂ق�������
	coll->LookAtTarget();
	//VC
	coll->CharacterVC("Attack");
}

BossMuscleStateSwingAttack::~BossMuscleStateSwingAttack()
{
	auto coolTime = kAttackCoolTime;
	//�{���ԂȂ�N�[���^�C����Z������
	if (m_isAngry)coolTime *= 0.5f;
	//�U���̃N�[���^�C��
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetAttackCoolTime(coolTime);
	if (!m_attack.expired())m_attack.lock()->Delete();
	//�O���[�v�ɏ������Ă���Ȃ�U����������
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossMuscleStateSwingAttack::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossMuscleStateSwingAttack::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
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
		ChangeState(std::make_shared<BossMuscleStateHit>(m_owner, m_isAngry));
		return;
	}
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		CreateAttack(actorManager);
	}
	//�A�j���[�V�����I����
	if (coll->GetModel()->IsFinishAnim())
	{
		//�ҋ@��Ԃɖ߂�
		ChangeState(std::make_shared<BossMuscleStateIdle>(m_owner, m_isAngry));
		return;
	}
	//�U���̈ʒu�X�V
	if (!m_attack.expired()) UpdateAttackPos();

	//�ړ��t���[�����͑O�ɐi��
	if (m_attackCountFrame <= m_attackData.moveFrame)
	{
		//�v���C���[�������ꍇ�̂�
		auto targetData = coll->GetTargetData();
		if (targetData.targetDis >= kAttackMoveDistance)
		{
			//�O�i
			AttackMove();
		}
	}
	else
	{
		//����
		coll->GetRb()->SpeedDown(kMoveDeceRate);
	}
}

void BossMuscleStateSwingAttack::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//�쐬�ƎQ��
	m_attack = std::dynamic_pointer_cast<AreaOfEffectAttack>(actorManager.lock()->CreateAttack(AttackType::AreaOfEffect, m_owner).lock());
	//�U�����쐬
	auto attack = m_attack.lock();
	auto data = m_attackData;
	//�傫��
	attack->SetRadius(data.radius);
	//�_���[�W�A�����t���[���A�m�b�N�o�b�N�̑傫���A�U���̏d���A�q�b�g�X�g�b�v�̒����A�J�����̗h��
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
}

void BossMuscleStateSwingAttack::UpdateAttackPos()
{
	auto model = m_owner.lock()->GetModel();
	//��̍��W
	VECTOR hand = MV1GetFramePosition(model->GetModelHandle(), kRightHandIndex);//��̎w��
	//���W���Z�b�g
	m_attack.lock()->SetPos(hand);
}

void BossMuscleStateSwingAttack::AttackMove()
{
	//�����Ă�����Ɉړ�
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//�{���ԂɂȂ��Ă���Ȃ瑬�x2�{
	float moveSpeed = m_attackData.moveSpeed;
	if (m_isAngry) moveSpeed = moveSpeed * kAngryMoveSpeedRate;
	coll->GetRb()->SetMoveVec(coll->GetModel()->GetDir() * moveSpeed);
}