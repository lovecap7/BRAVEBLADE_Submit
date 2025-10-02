#include "BossMuscleStateRandMove.h"
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
	const char* kAnim = "Boss|Run";
	//���x
	constexpr float kMoveSpeed = 9.0f;
	//�ړ��t���[��
	constexpr int kMoveFrame = 120;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 30;
}

BossMuscleStateRandMove::BossMuscleStateRandMove(std::weak_ptr<Actor> owner, bool isAngry):
	BossMuscleStateBase(owner, isAngry),
	m_moveFrame(kMoveFrame)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetCollState(CollisionState::Move);
	//�A�j���[�V����
	coll->GetModel()->SetAnim(kAnim, true);
	//�����_���ȕ����Ɉړ�
	m_moveVec = Vector3::GetRandVecXZ() * kMoveSpeed;
	coll->GetRb()->SetMoveVec(m_moveVec);
	//���f���̌���
	coll->GetModel()->SetDir(m_moveVec.XZ());
}
BossMuscleStateRandMove::~BossMuscleStateRandMove()
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

void BossMuscleStateRandMove::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossMuscleStateRandMove::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
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
	--m_moveFrame;
	if(m_moveFrame < 0 || coll->IsWall())
	{
		//�ҋ@���
		ChangeState(std::make_shared<BossMuscleStateIdle>(m_owner, m_isAngry));
		return;
	}
	//�ړ�
	coll->GetRb()->SetMoveVec(m_moveVec);
	//���f���̌���
	coll->GetModel()->SetDir(m_moveVec.XZ());
}
