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
#include "../../../../../General/Effect/EffekseerManager.h"
namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V�����̖��O
	const char* kAnim = "Boss|Change";
}

BossMuscleStateAngry::BossMuscleStateAngry(std::weak_ptr<Actor> owner) :
	BossMuscleStateBase(owner,true)
{
	//�ҋ@���
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Normal);
	//VC
	coll->CharacterVC("Angry");
}

BossMuscleStateAngry::~BossMuscleStateAngry()
{
}

void BossMuscleStateAngry::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
	//�{��G�t�F�N�g
	EffekseerManager::GetInstance().CreateTrackActorEffect("BossAngryEff", m_owner);
}

void BossMuscleStateAngry::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�R���C�_�u��
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//���S
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BossMuscleStateDeath>(m_owner));
		return;
	}
	//�A�j���[�V�����I����
	if (coll->GetModel()->IsFinishAnim())
	{
		//�{��ҋ@��Ԃ�
		ChangeState(std::make_shared<BossMuscleStateIdle>(m_owner,m_isAngry));
		return;
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
