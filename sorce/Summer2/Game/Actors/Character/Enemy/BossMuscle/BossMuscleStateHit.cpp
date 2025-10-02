#include "BossMuscleStateHit.h"
#include "BossMuscleStateAngry.h"
#include "BossMuscleStateDeath.h"
#include "BossMuscleStateIdle.h"
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
	//�A�j���[�V����
	const char* kAnim = "Boss|Hit";
}
BossMuscleStateHit::BossMuscleStateHit(std::weak_ptr<Actor> owner, bool isAngry) :
	BossMuscleStateBase(owner,isAngry)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//����
	coll->GetModel()->SetAnim(kAnim, false);
	//VC
	coll->CharacterVC("Damage");
}

BossMuscleStateHit::~BossMuscleStateHit()
{
}

void BossMuscleStateHit::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossMuscleStateHit::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
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
	auto model = coll->GetModel();
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		//���߂���
		model->ReplayAnim();
	}
	//���f���̃A�j���[�V�������I�������
	if (model->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<BossMuscleStateIdle>(m_owner, m_isAngry));
		return;
	}

	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
