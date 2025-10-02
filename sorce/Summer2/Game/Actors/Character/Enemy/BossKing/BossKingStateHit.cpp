#include "BossKingStateHit.h"
#include "BossKingStateDeath.h"
#include "BossKingStateIdle.h"
#include "BossKingStateChange.h"
#include "BossKing.h"
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
	const char* kAnim = "Boss3|Hit";
}

BossKingStateHit::BossKingStateHit(std::weak_ptr<Actor> owner, bool isTransformSecond):
	BossKingStateBase(owner,isTransformSecond)
{
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	//����
	coll->GetModel()->SetAnim(kAnim, false);
	//VC
	coll->CharacterVC("Damage");
}

BossKingStateHit::~BossKingStateHit()
{
}

void BossKingStateHit::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossKingStateHit::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
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
		ChangeState(std::make_shared<BossKingStateIdle>(m_owner,m_isTransformSecond));
		return;
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}