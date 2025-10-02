#include "BossMuscleStateDeath.h"
#include "BossMuscle.h"
#include "../EnemyBase.h"
#include "../../../Actor.h"
#include "../../../ActorManager.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../General/Sound/SoundManager.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"

namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V����
	const char* kAnim = "Boss|Dead";
	//�A�j���[�V�������x
	constexpr float kAnimSpeed = 0.6f;
}

BossMuscleStateDeath::BossMuscleStateDeath(std::weak_ptr<Actor> owner) :
	BossMuscleStateBase(owner,true)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//���S��Ԃɂ���
	coll->SetCollState(CollisionState::Dead);
	//���S
	coll->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//���G
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
	//BGM���~�߂�
	SoundManager::GetInstance().StopBGM();
	//VC
	coll->CharacterVC("Dead");
}

BossMuscleStateDeath::~BossMuscleStateDeath()
{
}

void BossMuscleStateDeath::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossMuscleStateDeath::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//�A�j���[�V�����I����
	if (coll->GetModel()->IsFinishAnim())
	{
		coll->Delete();//�폜
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
