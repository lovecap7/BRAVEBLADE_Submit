#include "PurpleDinosaurStateHit.h"
#include "PurpleDinosaurStateIdle.h"
#include "PurpleDinosaurStateDeath.h"
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
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|HitReact";//����炢
}

PurpleDinosaurStateHit::PurpleDinosaurStateHit(std::weak_ptr<Actor> owner):
	PurpleDinosaurStateBase(owner)
{
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	auto model = coll->GetModel();
	//����
	model->SetAnim(kAnim, false);
	//VC
	coll->CharacterVC("Damage");
	//�x��
	coll->SetIsWarning(true);
}

PurpleDinosaurStateHit::~PurpleDinosaurStateHit()
{
}

void PurpleDinosaurStateHit::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateHit::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	//���S
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<PurpleDinosaurStateDeath>(m_owner));
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
		ChangeState(std::make_shared<PurpleDinosaurStateIdle>(m_owner));
		return;
	}

	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}