#include "BossKingStateRapidFire.h"
#include "BossKingStateIdle.h"
#include "BossKingStateHit.h"
#include "BossKingStateDeath.h"
#include "BossKingStateChase.h"
#include "BossKingStateChange.h"
#include "BossKing.h"
#include "../EnemyBase.h"
#include "../../Player/Player.h"
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
#include "../../../Attack/HomingBullet.h"
#include "../../../../../General/Effect/EffekseerManager.h"

namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 30;
	//�E��̃n���h��
	constexpr int kRightHandIndex = 63;
	
	//�e�̍���
	constexpr float kBulletPosY = 150.0f;
	//�ǐ՗�
	constexpr float kTrackingRate = 0.05f;
	//�����Ԋu
	constexpr int kShotDelayFrame = 10;
}


BossKingStateRapidFire::BossKingStateRapidFire(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager):
	BossKingStateBase(owner, true),
	m_attackCountFrame(0),
	m_shotNum(1)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kRapidFireName);
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//�U��
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//����̂ق�������
	coll->LookAtTarget();
	//VC
	RandAttackVC();
}

BossKingStateRapidFire::~BossKingStateRapidFire()
{
	//�U���̃N�[���^�C��
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
	//�O���[�v�ɏ������Ă���Ȃ�U����������
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossKingStateRapidFire::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossKingStateRapidFire::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
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
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossKingStateHit>(m_owner, m_isTransformSecond));
		return;
	}
	//�J�E���g
	++m_attackCountFrame;
	//�U�������t���[��
	if (m_attackCountFrame == m_attackData.startFrame + kShotDelayFrame * m_shotNum)
	{
		//�ł��������J�E���g
		++m_shotNum;
		m_shotNum = MathSub::ClampInt(m_shotNum, 0, m_attackData.attackNum);
		CreateAttack(actorManager);
	}
	//�A�j���[�V�����I����
	if (coll->GetModel()->IsFinishAnim())
	{
		//�ҋ@���
		ChangeState(std::make_shared<BossKingStateIdle>(m_owner, m_isTransformSecond));
		return;
	}

	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossKingStateRapidFire::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//�쐬�ƎQ��
	auto attack = std::dynamic_pointer_cast<HomingBullet>(actorManager.lock()->CreateAttack(AttackType::Homing, m_owner).lock());
	//�e�̐ݒ�
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	auto data = m_attackData;
	//�傫��
	attack->SetRadius(data.radius);
	//�_���[�W�A�����t���[���A�m�b�N�o�b�N�̑傫���A�U���̏d���A�q�b�g�X�g�b�v�̒����A�J�����̗h��
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
	//�E��ƉE��
	auto model = coll->GetModel();
	auto rightHandPos = MV1GetFramePosition(model->GetModelHandle(), kRightHandIndex);
	//�����ʒu
	Vector3 bulletPos = rightHandPos;
	//����
	bulletPos.y = kBulletPosY;
	attack->SetPos(bulletPos);
	//�e�̐i�s�����ƃX�s�[�h
	auto vec = Vector3::Up();
	attack->SetVec(vec);
	//�^�[�Q�b�g
	attack->SetTarget(actorManager.lock()->GetPlayer());
	//�ǐ՗�
	attack->SetTrackingRate(kTrackingRate);
	//���x
	attack->SetSpeed(data.moveSpeed);
	//�G�t�F�N�g
	EffekseerManager::GetInstance().CreateTrackActorEffect("BreathEff", attack);
}


