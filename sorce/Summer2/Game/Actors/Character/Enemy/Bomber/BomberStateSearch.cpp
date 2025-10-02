#include "BomberStateSearch.h"
#include "BomberStateDeath.h"
#include "BomberStateIdle.h"
#include "BomberStateHit.h"
#include "Bomber.h"
#include "../EnemyBase.h"
#include "../../../ActorManager.h"
#include "../../../SearchPlace.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../General/Collision/Physics.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
namespace
{
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Walk";
	//���x
	constexpr float kMoveSpeed = 1.0f;
	//�ړ��t���[��
	constexpr int kMoveFrame = 300;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 5;
	//���C�L���X�g�̋��e�l(�����ƃ^�[�Q�b�g)
	constexpr int kRayCastToleranceNum = 2;
	//�ړ��O���C�L���X�g�̋��e�l(����)
	constexpr int kBeforeMoveToleranceNum = 1;
	//�ړ���̃��C�L���X�g�̋��e�l
	constexpr int kAfterMoveToleranceNum = 0;
}

BomberStateSearch::BomberStateSearch(std::weak_ptr<Actor> owner) :
	BomberStateBase(owner),
	m_moveFrame(kMoveFrame)
{
	if (m_owner.expired())return;
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	//�A�j���[�V����
	coll->GetModel()->SetAnim(kAnim, true);
	//���
	coll->SetCollState(CollisionState::Move);
	//�����_���ȕ����Ɉړ�
	m_moveVec = Vector3::GetRandVecXZ() * kMoveSpeed;
	coll->GetRb()->SetMoveVec(m_moveVec);
	//���f���̌���
	coll->GetModel()->SetDir(m_moveVec.XZ());
	//�x���͂��Ă��Ȃ�
	coll->SetIsWarning(false);
}

BomberStateSearch::~BomberStateSearch()
{
}

void BomberStateSearch::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BomberStateSearch::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
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
	//�ǂɓ���������
	if (coll->IsWall() || m_moveFrame < 0.0f)
	{
		m_moveFrame = kMoveFrame;
		//�����_���ȕ����Ɉړ�
		m_moveVec = Vector3::GetRandVecXZ() * kMoveSpeed;
	}
	//�^�[�Q�b�g
	auto targetData = coll->GetTargetData();
	//���f��
	auto model = coll->GetModel();
	//���G�͈͊O�ɏo����
	if (!coll->GetSearchPlace().expired())
	{
		std::shared_ptr<SearchPlace> searchPlace = coll->GetSearchPlace().lock();
		//�͈͊O
		if (!searchPlace->IsInSearchPlace(coll->GetPos()))
		{
			//�ڕW�n�_
			Vector3 searchPlacePos = searchPlace->GetPos();
			//�͈͓��Ɍ������ē���
			m_moveVec = searchPlacePos - coll->GetPos();
			//�ړ��x�N�g��(�Ԃɏ�Q��������ꍇ������)
			m_moveVec = GetNextNomVecFromRayCast(coll, searchPlacePos, m_moveVec, kMoveSpeed, kBeforeMoveToleranceNum, kAfterMoveToleranceNum);
		}
	}
	//�ړ��t���[��
	--m_moveFrame;
	//�v���C���[��������
	if (targetData.isHitTarget)
	{
		//�Ղ镨���Ȃ��Ȃ�
		if (!Physics::GetInstance().RayCast(coll->GetPos(), targetData.targetPos).size() <= kRayCastToleranceNum)
		{
			//�ҋ@���
			ChangeState(std::make_shared<BomberStateIdle>(m_owner));
			return;
		}
	}
	//���f���̌���
	model->SetDir(m_moveVec.XZ());
	//�ړ�
	coll->GetRb()->SetMoveVec(model->GetDir() * kMoveSpeed);
}
