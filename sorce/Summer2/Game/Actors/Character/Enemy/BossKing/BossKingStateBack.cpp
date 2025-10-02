#include "BossKingStateBack.h"
#include "BossKingStateIdle.h"
#include "BossKingStateHit.h"
#include "BossKingStateDeath.h"
#include "BossKingStateChase.h"
#include "BossKingStateChange.h"
#include "BossKing.h"
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
	const char* kAnim = "Boss3|WalkBack";
	//���x
	constexpr float kBackSpeed = 9.0f;
	//�ړ��t���[��
	constexpr int kMoveFrame = 60;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 5;
}
BossKingStateBack::BossKingStateBack(std::weak_ptr<Actor> owner):
	BossKingStateBase(owner, false),
	m_moveFrame(kMoveFrame)
{
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetCollState(CollisionState::Move);
	//�A�j���[�V����
	coll->GetModel()->SetAnim(kAnim, true);
}
BossKingStateBack::~BossKingStateBack()
{
	auto coolTime = kAttackCoolTime;
	//�U���̃N�[���^�C��
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetAttackCoolTime(coolTime);
	//�O���[�v�ɏ������Ă���Ȃ�U����������
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossKingStateBack::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossKingStateBack::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�R���C�_�u��
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	auto targetData = coll->GetTargetData();
	//���S
	if (coll->GetHitPoints().lock()->IsDead())
	{
		//���`�ԂȂ�
		if (m_isTransformSecond)
		{
			//���S
			ChangeState(std::make_shared<BossKingStateDeath>(m_owner));
			return;
		}
		else
		{
			//�ϐg
			ChangeState(std::make_shared<BossKingStateChange>(m_owner));
			return;
		}
	}
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossKingStateHit>(m_owner, m_isTransformSecond));
		return;
	}
	--m_moveFrame;
	if (m_moveFrame < 0 || coll->IsWall())
	{
		//�ҋ@���
		ChangeState(std::make_shared<BossKingStateIdle>(m_owner, m_isTransformSecond));
		return;
	}
	//�v���C���[������
	coll->LookAtTarget();
	//������
	coll->GetRb()->SetMoveVec(targetData.targetDirXZ * -kBackSpeed);
}
