#include "BossMuscleStateBeam.h"
#include "BossMuscleStateIdle.h"
#include "BossMuscleStateAngry.h"
#include "BossMuscleStateDeath.h"
#include "BossMuscleStateHit.h"
#include "BossMuscle.h"
#include "../../../ActorManager.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Collision/SphereCollider.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../../General/Effect/MyEffect.h"
#include "../../../Attack/ULT.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"

namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�U�����胊�Z�b�g�̕p�x
	constexpr int kBeamResetFrame = 7;
	//�E��ƍ���̃C���f�b�N�X
	constexpr int kRightHandIndex = 57;
	constexpr int kLeftHandIndex = 29;
	//���̍U���t���[��
	constexpr int kAttackCoolTime = 150;
	//�{���Ԃ̎��̑��x2�{
	constexpr float kAngryMoveSpeedRate = 2.0f;
	//���f���̌�������
	const Vector2 kAdjustDir = Vector2(0.05f, 0.0f);
	//�r�[���̎˒�
	constexpr float kBeamRange = 5000.0f;
	//�����̍X�V�t���[������
	constexpr int kUpdateDirFrame = 60;
}

BossMuscleStateBeam::BossMuscleStateBeam(std::weak_ptr<Actor> owner, bool isAngry, const std::weak_ptr<ActorManager> actorManager):
	BossMuscleStateBase(owner, isAngry),
	m_attackCountFrame(0)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kBeamName);
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//�U��
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//����̂ق�������
	coll->LookAtTarget();
	//�`���[�W�G�t�F�N�g
	m_beamChargeEff = EffekseerManager::GetInstance().CreateEffect("BossBeamChargeEff", m_owner.lock()->GetPos());
	//VC
	coll->CharacterVC("Attack");
}

BossMuscleStateBeam::~BossMuscleStateBeam()
{
	int coolTime = kAttackCoolTime;
	//�{���ԂȂ�N�[���^�C����Z������
	if (m_isAngry)coolTime /= 2;
	//�U���̃N�[���^�C��
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetAttackCoolTime(coolTime);
	//�G�t�F�N�g�̍폜
	if (!m_beamChargeEff.expired())
	{
		m_beamChargeEff.lock()->Delete();
	}
	if( !m_beamEff.expired())
	{
		m_beamEff.lock()->Delete();
	}
	//�U���̍폜
	if (!m_beam.expired())
	{
		m_beam.lock()->Delete();
	}
	//�O���[�v�ɏ������Ă���Ȃ�U����������
	if (coll->IsInGroup())coll->SetCanAttack(false);
}

void BossMuscleStateBeam::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossMuscleStateBeam::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//���S
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BossMuscleStateDeath>(m_owner));
		return;
	}
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossMuscleStateHit>(m_owner, m_isAngry));
		return;
	}
	//�J�E���g
	++m_attackCountFrame;
	//�r�[���̃`���[�W�G�t�F�N�g�̈ʒu�X�V
	auto model = coll->GetModel();
	Vector3 rightHand = MV1GetFramePosition(model->GetModelHandle(), kRightHandIndex);
	Vector3 leftHand = MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex);
	VECTOR pos = ((rightHand + leftHand) * 0.5f).ToDxLibVector(); //����̒��Ԉʒu
	if (!m_beamChargeEff.expired())
	{
		m_beamChargeEff.lock()->SetPos(pos);
	}
	//�U�������t���[��
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		CreateAttack(pos,actorManager);
	}
	//�����O�܂Ō�����������ύX
	else if (m_attackCountFrame < kUpdateDirFrame || m_isAngry)
	{
		//����̂ق�������
		coll->LookAtTarget();
	}
	//�{���Ԃ̎��͒Ǐ]������
	if (m_isAngry)
	{
		//�ʒu�X�V
		UpdateBeamPos(pos, model, coll);
	}

	//�U����������Z�b�g
	if (m_attackCountFrame % kBeamResetFrame == 0)
	{
		if (!m_beam.expired())m_beam.lock()->ResetHitId();
	}
	//�A�j���[�V�����I����
	if (coll->GetModel()->IsFinishAnim())
	{
		//�ҋ@��Ԃɖ߂�
		ChangeState(std::make_shared<BossMuscleStateIdle>(m_owner,m_isAngry));
		return;
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossMuscleStateBeam::UpdateBeamPos(const Vector3 pos, std::shared_ptr<Model> model, std::shared_ptr<BossMuscle> coll)
{
	//�G�t�F�N�g�̈ʒu
	Vector3 startPos = (pos + coll->GetPos()) * 0.5f;
	startPos.y = pos.y;
	//�U���̌���
	auto dir = model->GetDir().XZ() + kAdjustDir;
	if (!m_beamEff.expired())
	{
		//�K�E�G�t�F�N�g
		auto eff = m_beamEff.lock();
		eff->SetPos(startPos);
		eff->LookAt(dir.XZ());
	}
	if (!m_beam.expired())
	{
		auto beem = m_beam.lock();
		//�U���̈ʒu
		Vector3 endPos = startPos + (dir.XZ() * kBeamRange);
		//���W���Z�b�g
		beem->SetStartPos(startPos);
		beem->SetEndPos(endPos);
	}
}

void BossMuscleStateBeam::CreateAttack(VECTOR pos, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//�쐬�ƎQ��
	auto attack = std::dynamic_pointer_cast<ULT>(actorManager.lock()->CreateAttack(AttackType::ULT, m_owner).lock());
	//�U�����쐬
	auto data = m_attackData;
	//�傫��
	attack->SetRadius(data.radius);
	//�_���[�W�A�����t���[���A�m�b�N�o�b�N�̑傫���A�U���̏d���A�q�b�g�X�g�b�v�̒����A�J�����̗h��
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
	m_beam = attack; //�U���̎Q�Ƃ�ۑ�
	//�r�[���G�t�F�N�g
	m_beamEff = EffekseerManager::GetInstance().CreateEffect("BossBeamEff", m_owner.lock()->GetPos());
	//�U���̈ʒu
	UpdateBeamPos(pos, coll->GetModel(), coll);
}
