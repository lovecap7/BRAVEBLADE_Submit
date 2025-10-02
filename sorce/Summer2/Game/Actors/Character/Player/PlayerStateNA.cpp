#include "PlayerStateNA.h"
#include "PlayerStateIdle.h"
#include "PlayerStateRolling.h"
#include "PlayerStateCharge.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "UltGage.h"
#include "../Enemy/EnemyBase.h"
#include "../../ActorManager.h"
#include "../../Attack/Slash.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../General/Effect/MyEffect.h"
#include "../../../../Game/Camera/GameCamera/GameCamera.h"

namespace
{
	//����̍��W�Ɠ����蔻��̏��
	//�E��̖�w�̃C���f�b�N�X
	constexpr int kRightRingFingerIndex = 55;
	constexpr int kRightIndexFingerIndex = 43;
	//����̒���
	constexpr float kSwordHeight = 150.0f;
	//�L�����Z��
	constexpr int kAttackCancelFrame = 15;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A���U���̍ő吔
	constexpr int kMaxComboNum = 4;
	//�U���������Ȃ�i�K��
	constexpr int kHighAttackComboNum = 3;
	//�O�i�����鋗��
	constexpr float kAttackMoveDistance = 100.0f;
	//�S�g���ɘa���銄��
	constexpr float kNearDistanceAttackMoveRate = 0.1f;
}

PlayerStateNA::PlayerStateNA(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager) :
	PlayerStateBase(player),
	m_comboNum(1),
	m_attackCountFrame(0)
{
	//VC
	RandAttackVC();
	//�U���f�[�^�̏�����
	InitAttackData(actorManager);
	//�Ռ��G�t�F�N�g
	EffekseerManager::GetInstance().CreateEffect("LowShockWaveEff", m_owner.lock()->GetPos());
}

PlayerStateNA::~PlayerStateNA()
{
	if (!m_eff.expired())
	{
		m_eff.lock()->Delete();
	}
}
void PlayerStateNA::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}
void PlayerStateNA::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//�{�X��|��
	if (actorManager.lock()->IsBossDisappear())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//�{�X�̗̑͂��Ȃ��Ȃ����ꍇ�܂��̓��f���̃A�j���[�V�������I�������
	if (actorManager.lock()->IsBossDead() ||
		coll->GetModel()->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//���S�������{�X���|���ĂȂ��ꍇ
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_owner));
		return;
	}
	//�U�����󂯂��Ȃ�
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_owner));
		return;
	}
	auto& input = Input::GetInstance();
	//�J�E���g
	++m_attackCountFrame;
	auto model = coll->GetModel();
	//�U�������t���[��
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		//SE
		coll->CharacterOnceSE("NA");
		//�U���쐬
		CreateAttack(m_attackData.radius, m_attackData.damege, m_attackData.keepFrame, 
			m_attackData.knockBackPower, m_attackData.attackWeight, actorManager);
		if (!m_eff.expired())
		{
			//�G�t�F�N�g�폜
			m_eff.lock()->Delete();
		}
		//�a���G�t�F�N�g��i�K�ɉ����č쐬
		if (m_comboNum >= kHighAttackComboNum)
		{
			//�a��2�G�t�F�N�g
			m_eff = EffekseerManager::GetInstance().CreateEffect("SlashtTraject2Eff", m_owner.lock()->GetPos());
		}
		else
		{
			//�a��1�G�t�F�N�g
			m_eff = EffekseerManager::GetInstance().CreateEffect("SlashtTraject1Eff", m_owner.lock()->GetPos());
		}
	}
	//�A�j���[�V�����̃��X�g���t���[���ȓ��œ��͂�����Ȃ�2�i��ڂ̍U��
	if (model->GetTotalAnimFrame() - kAttackCancelFrame <= model->GetNowAnimFrame())
	{
		//����{�^������������
		if (input.IsTrigger("A"))
		{
			//���
			ChangeState(std::make_shared<PlayerStateRolling>(m_owner));
			return;
		}
		//�܂��U���ł���Ȃ�
		if (input.IsTrigger("X") && m_comboNum < kMaxComboNum)
		{
			//���̍U��
			++m_comboNum;
			//������
			InitAttackData(actorManager);
			//�Ռ��G�t�F�N�g
			EffekseerManager::GetInstance().CreateEffect("LowShockWaveEff", m_owner.lock()->GetPos());
			m_attackCountFrame = 0;
			return;
		}
		//�`���[�W�{�^������������
		if (input.IsTrigger("Y"))
		{
			//�`���[�W
			ChangeState(std::make_shared<PlayerStateCharge>(m_owner, actorManager));
			return;
		}
	}
	//�U���̈ʒu�X�V
	UpdateAttackPos();
	//�ړ��t���[�����͑O�ɐi��
	if (m_attackCountFrame <= m_attackData.moveFrame)
	{
		//���x
		float speed = m_attackData.moveSpeed;
		//�^�[�Q�b�g
		CharacterBase::TargetData targetData = coll->GetTargetData();
		//�^�[�Q�b�g���߂��Ƃ��͑S�g���x�𗎂Ƃ�
		if (targetData.targetDis <= kAttackMoveDistance && targetData.isHitTarget)
		{
			speed *= kNearDistanceAttackMoveRate;
		}
		//�O�i
		AttackMove(speed);
	}
	else
	{
		//����
		coll->GetRb()->SpeedDown(kMoveDeceRate);
	}
}
void PlayerStateNA::CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager)
{
	auto owner = m_owner.lock();
	//�쐬�ƎQ��
	m_attack = std::dynamic_pointer_cast<Slash>(actorManager.lock()->CreateAttack(AttackType::Slash, m_owner).lock());
	//�U�����쐬
	auto attack = m_attack.lock();
	auto data = m_attackData;
	//�傫��
	attack->SetRadius(data.radius);
	//�_���[�W�A�����t���[���A�m�b�N�o�b�N�̑傫���A�U���̏d���A�q�b�g�X�g�b�v�̒����A�J�����̗h��
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
}
void PlayerStateNA::UpdateAttackPos()
{
	auto model = m_owner.lock()->GetModel();
	//����
	//�E��̖�w�Ɛl�����w�̍��W���畐��̍��W���o��
	VECTOR ringFinger = MV1GetFramePosition(model->GetModelHandle(), kRightRingFingerIndex);//��w
	VECTOR indexFinger = MV1GetFramePosition(model->GetModelHandle(), kRightIndexFingerIndex);//�l�����w
	//����̖���
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	VECTOR swordTip = VAdd(ringFinger, VScale(swordDir, kSwordHeight));//������̍��W�ɉ��Z���Č���̍��W���o��
	if (!m_attack.expired())
	{
		//���W���Z�b�g
		m_attack.lock()->SetStartPos(ringFinger);
		m_attack.lock()->SetEndPos(swordTip);
		if (!m_eff.expired())
		{
			auto eff = m_eff.lock();
			//�G�t�F�N�g�̈ʒu�X�V
			eff->SetPos(((Vector3(ringFinger) + swordTip) / 2.0f));
		}
	}
}
void PlayerStateNA::AttackMove(float speed)
{
	//�^�[�Q�b�g�����G���Ă�Ȃ�(�^�[�Q�b�g�̂ق�������)
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto targetData = coll->GetTargetData();
	//����
	Vector3 dir = coll->GetPlayerStickVec().XZ();
	//�^�[�Q�b�g�������ł�����
	if (targetData.isHitTarget)
	{
		dir = targetData.targetDirXZ;
	}
	//�����̍X�V
	coll->GetModel()->SetDir(dir.XZ());
	//�����Ă�����Ɉړ�
	coll->GetRb()->SetMoveVec(coll->GetModel()->GetDir() * speed);
}

void PlayerStateNA::InitAttackData(const std::weak_ptr<ActorManager> actorManager)
{
	std::string attackName;
	//�U���̒i�K�ɍ��킹�čU���̏�����
	switch (m_comboNum)
	{
	case 1:
		attackName = kNA1Name;
		break;
	case 2:
		attackName = kNA2Name;
		break;
	case 3:
		attackName = kNA3Name;
		break;
	case kMaxComboNum:
		attackName = kNA4Name;
		//VC
		RandAttackVC();
		break;
	default:
		break;
	}
	m_attackData = actorManager.lock()->GetAttackData(kPlayerName, attackName);
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//�ʏ�U��
	coll->SetCollState(CollisionState::Normal);
	//�U��
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//���Z�Q�[�W�̗\��
	coll->GetUltGage().lock()->SetPendingUltGage(m_attackData.addUltGage);
}