#include "BossMuscleStateJumpAttack.h"
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
#include "../../../Attack/WaveAttack.h"
#include "../../../../../General/Effect/EffekseerManager.h"

namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 180;
	//�f�B���C1
	constexpr int kDelay1Frame = 40;
	//�f�B���C2
	constexpr int kDelay2Frame = 80;

}

BossMuscleStateJumpAttack::BossMuscleStateJumpAttack(std::weak_ptr<Actor> owner, bool isAngry, const std::weak_ptr<ActorManager> actorManager):
	BossMuscleStateBase(owner, isAngry),
	m_attackCountFrame(0)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kJumpAttackName);
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//�U��
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//����̂ق�������
	coll->LookAtTarget();
	//VC
	coll->CharacterVC("Attack");
}

BossMuscleStateJumpAttack::~BossMuscleStateJumpAttack()
{
	auto coolTime = kAttackCoolTime;
	//�{���ԂȂ�N�[���^�C����Z������
	if (m_isAngry)coolTime *= 0.5f;
	//�U���̃N�[���^�C��
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetAttackCoolTime(coolTime);
	//�O���[�v�ɏ������Ă���Ȃ�U����������
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossMuscleStateJumpAttack::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossMuscleStateJumpAttack::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
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
	//�A�j���[�V�����I����
	if (coll->GetModel()->IsFinishAnim())
	{
		//�ҋ@��Ԃɖ߂�
		ChangeState(std::make_shared<BossMuscleStateIdle>(m_owner, m_isAngry));
		return;
	}
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		CreateAttack(actorManager);
	}
	//�{���Ԃ̎�
	if (m_isAngry)
	{
		if (m_attackCountFrame == m_attackData.startFrame + kDelay1Frame)
		{
			CreateAttack(actorManager);
		}
		if (m_attackCountFrame == m_attackData.startFrame + kDelay2Frame)
		{
			CreateAttack(actorManager);
		}
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossMuscleStateJumpAttack::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//�쐬�ƎQ��
	auto attack = std::dynamic_pointer_cast<WaveAttack>(actorManager.lock()->CreateAttack(AttackType::Wave, m_owner).lock());
	//�U�����쐬
	auto data = m_attackData;
	//�傫��
	attack->SetRadius(data.radius);
	//���_
	attack->SetPos(m_owner.lock()->GetPos());
	//�g�̍L���鑬�x
	attack->SetWaveSpeed(data.moveSpeed);
	//�_���[�W�A�����t���[���A�m�b�N�o�b�N�̑傫���A�U���̏d���A�q�b�g�X�g�b�v�̒����A�J�����̗h��
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
	//�K�E�G�t�F�N�g
	auto eff = EffekseerManager::GetInstance().CreateEffect("WaveAttackEff", m_owner.lock()->GetPos());
}

