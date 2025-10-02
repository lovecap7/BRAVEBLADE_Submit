#include "BossKingStateBlastAttack.h"
#include "BossKingStateIdle.h"
#include "BossKingStateHit.h"
#include "BossKingStateDeath.h"
#include "BossKingStateChase.h"
#include "BossKingStateChange.h"
#include "BossKing.h"
#include "../EnemyBase.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../ActorManager.h"
#include "../../../Character/Player/Player.h"
#include "../../../Attack/Blast.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../../General/Effect/MyEffect.h"


namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 80;
	//�ǐՏI���t���[��
	constexpr int kTrackFrameOffset = 10;
}

BossKingStateBlastAttack::BossKingStateBlastAttack(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager) :
	BossKingStateBase(owner, false),
	m_attackCountFrame(0)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kBlastAttackName);
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//�U��
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//����̂ق�������
	coll->LookAtTarget();
	//�K�E�G�t�F�N�g
	auto player = actorManager.lock()->GetPlayer();
	if (!player.expired())
	{
		m_blastMagicEff = EffekseerManager::GetInstance().CreateEffect("BlastMagicEff", player.lock()->GetPos());
	}
	//VC
	RandAttackVC();
}

BossKingStateBlastAttack::~BossKingStateBlastAttack()
{
	auto coolTime = kAttackCoolTime;
	//�U���̃N�[���^�C��
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetAttackCoolTime(coolTime);
	//�G�t�F�N�g������
	if (!m_blastMagicEff.expired())
	{
		m_blastMagicEff.lock()->Delete();
	}
	//�O���[�v�ɏ������Ă���Ȃ�U����������
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossKingStateBlastAttack::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossKingStateBlastAttack::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�R���C�_�u��
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
	//�A�j���[�V�����I����
	if (coll->GetModel()->IsFinishAnim())
	{
		//�ҋ@��Ԃɖ߂�
		ChangeState(std::make_shared<BossKingStateIdle>(m_owner, false));
		return;
	}
	//�J�E���g
	++m_attackCountFrame;
	//���@�w�̈ʒu�X�V
	auto player = actorManager.lock()->GetPlayer();
	if (!player.expired())
	{
		//�����O�܂Œǐ�
		if (m_attackCountFrame <= m_attackData.startFrame - kTrackFrameOffset)
		{
			UpdateBlastMagicEffPos(player.lock()->GetPos());
		}
	}
	//�U�������t���[��
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		if (!m_blastMagicEff.expired())
		{
			//�G�t�F�N�g�̈ʒu�ɍU�����o��
			auto model = coll->GetModel();
			CreateAttack(actorManager, m_blastMagicEff.lock()->GetPos());
			//�G�t�F�N�g������
			m_blastMagicEff.lock()->Delete();
		}
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossKingStateBlastAttack::CreateAttack(const std::weak_ptr<ActorManager> actorManager, Vector3 pos)
{
	//�쐬�ƎQ��
	auto attack = std::dynamic_pointer_cast<Blast>(actorManager.lock()->CreateAttack(AttackType::Blast, m_owner).lock());
	//�U�����쐬
	auto data = m_attackData;
	//�傫��
	attack->SetRadius(data.radius);
	//���_
	attack->SetPos(pos);
	//�_���[�W�A�����t���[���A�m�b�N�o�b�N�̑傫���A�U���̏d���A�q�b�g�X�g�b�v�̒����A�J�����̗h��
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
}

void BossKingStateBlastAttack::UpdateBlastMagicEffPos(Vector3 pos)
{
	if (!m_blastMagicEff.expired())
	{
		m_blastMagicEff.lock()->SetPos(pos);
	}
}
