#include "BossKingStateUlt.h"
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
#include "../../../../../General/Effect/MyEffect.h"

namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 30;
	//�e�̍���
	constexpr float kBulletPosY = 150.0f;
	//�ǐ՗�
	constexpr float kTrackingRate = 0.08f;
	//�����Ԋu
	constexpr int kShotDelayFrame = 20;
	//���@�w�̈ʒu����
	constexpr float kMagicCirclePosDis = 500.0f;
}

BossKingStateUlt::BossKingStateUlt(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager):
	BossKingStateBase(owner, true),
	m_attackCountFrame(0),
	m_shotNum(1)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kUltName);
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//�U��
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//����̂ق�������
	coll->LookAtTarget();
	//�O�㍶�E
	auto centerPos = coll->GetPos();
	Vector3 frontPos = Vector3::Forward() * kMagicCirclePosDis + centerPos;
	Vector3 backPos = Vector3::Back() * kMagicCirclePosDis + centerPos;
	Vector3 rightPos = Vector3::Right() * kMagicCirclePosDis + centerPos;
	Vector3 leftPos = Vector3::Left() * kMagicCirclePosDis + centerPos;
	//�G�t�F�N�g
	auto& effectManager = EffekseerManager::GetInstance();
	effectManager.CreateEffect("BigMagicCircleEff", centerPos);
	effectManager.CreateEffect("MagicCircleEff", frontPos);
	effectManager.CreateEffect("MagicCircleEff", backPos);
	effectManager.CreateEffect("MagicCircleEff", rightPos);
	effectManager.CreateEffect("MagicCircleEff", leftPos);
	//VC
	coll->CharacterVC("Ult");
	//SE
	coll->CharacterOnceSE("MagicCircle");
}
BossKingStateUlt::~BossKingStateUlt()
{
	//�U���̃N�[���^�C��
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
	//�O���[�v�ɏ������Ă���Ȃ�U����������
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossKingStateUlt::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossKingStateUlt::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
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
		//�O�㍶�E
		auto centerPos = coll->GetPos();
		Vector3 frontPos = Vector3::Forward() * kMagicCirclePosDis + centerPos;
		Vector3 backPos = Vector3::Back() * kMagicCirclePosDis + centerPos;
		Vector3 rightPos = Vector3::Right() * kMagicCirclePosDis + centerPos;
		Vector3 leftPos = Vector3::Left() * kMagicCirclePosDis + centerPos;
		CreateAttack(actorManager, frontPos);
		CreateAttack(actorManager, backPos);
		CreateAttack(actorManager, rightPos);
		CreateAttack(actorManager, leftPos);
	}
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		//VC
		coll->CharacterVC("Change");
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


void BossKingStateUlt::CreateAttack(const std::weak_ptr<ActorManager> actorManager, Vector3 pos)
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
	//�����ʒu
	Vector3 bulletPos = pos;
	//����
	bulletPos.y = kBulletPosY;
	attack->SetPos(bulletPos);
	//�e�̐i�s�����ƃX�s�[�h
	auto vec = Vector3::Up() + Vector3::GetRandVecXZ();
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

