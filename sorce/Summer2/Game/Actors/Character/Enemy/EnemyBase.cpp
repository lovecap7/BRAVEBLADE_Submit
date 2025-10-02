#include "EnemyBase.h"
#include "../CharacterStateBase.h"
#include "../Player/Player.h"
#include "../../../../General/Model.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Input.h"
#include "../../ActorManager.h"
#include "../../SearchPlace.h"
#include "../../../../Shader/MyDraw.h"

namespace
{
	//�v���C���[�Ƃ̋����������Ƃ��ɓ����蔻����~���鋗��
	const float kStopCollisionDistance = 4000.0f;
	//�ŏ��̃N�[���^�C��
	constexpr int kAttackCoolTime = 60;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//���������Ɣ��肷��Y���W
	constexpr float kStageFallY = -500.0f;
	//�x�����Ă��鎞�̍��G�����̔{��
	constexpr float kWarningDistanceRate = 1.4f;
}

EnemyBase::EnemyBase(Shape shape, EnemyGrade grade) :
	CharacterBase(shape),
	m_attackCoolTime(kAttackCoolTime),
	m_enemyGrade(grade),
	m_isActive(true),
	m_isWarning(false),
	m_name{}
{
}

void EnemyBase::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//���������ۂ̏���
	if (m_rb->m_pos.y < kStageFallY)
	{
		m_isDelete = true;
	}
	//�v���C���[���牓���Ȃ珈�������Ȃ�
	if (IsStopActiveCollision(actorManager))return;
#if _DEBUG
	if (m_enemyGrade == EnemyGrade::Boss)
	{
		//�{�X�����S������
		if (Input::GetInstance().IsTrigger("BossDead"))
		{
			m_hitPoints->SetIsNoDamege(false);
			m_hitPoints->Damage(999999);
		}
	}
#endif
	//�A�N�e�B�u��Ԃ���Ȃ��Ȃ�
	if (!m_isActive)
	{
		//����
		m_rb->SpeedDown(kMoveDeceRate);
		//�A�j���[�V�����̍X�V
		m_model->Update();
		return;
	}
	//�U���̃N�[���^�C�������炷
	UpdateAttackCoolTime();
	//�^�[�Q�b�g�𔭌��ł��������`�F�b�N
	auto target = actorManager.lock()->GetPlayer();
	if (!target.expired())
	{
		//����p
		float viewingAngle	 = m_viewingAngle;
		//���G����
		float searchDistance = m_searchDistance;
		//�x����ԂȂ�
		if (m_isWarning)
		{
			//�S����
			viewingAngle = MyMath::TwoPI_F;
			//���G�����������Ȃ�
			searchDistance *= kWarningDistanceRate;
		}
		//�͈͂Ǝ���p����^�[�Q�b�g�T��
		TargetSearch(searchDistance, viewingAngle, target.lock()->GetPos());
	}
	//��Ԃɍ��킹���X�V
	m_state->Update(camera, actorManager);
	//��Ԃ��ς���������`�F�b�N
	if (m_state != m_state->GetNextState())
	{
		//��Ԃ�ω�����
		m_state = m_state->GetNextState();
		m_state->Init();
	}
	//�A�j���[�V�����̍X�V
	m_model->Update();
	//�̗̓N���X�̃t���O���Z�b�g
	m_hitPoints->ResetHitFlags();
}

void EnemyBase::Draw() const
{
#if _DEBUG
	//DrawCapsule3D(
	//	m_rb->GetPos().ToDxLibVector(),
	//	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetEndPos().ToDxLibVector(),
	//	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetRadius(),
	//	16,
	//	0xff0000,
	//	0xff0000,
	//	false
	//);
	// 
	//���W
	auto pos = m_rb->m_pos;
	//����p
	float viewingAngle = m_viewingAngle;
	//���G����
	float searchDistance = m_searchDistance;
	//�x����ԂȂ�
	if (m_isWarning)
	{
		//�S����
		viewingAngle = MyMath::TwoPI_F;
		//���G�����������Ȃ�
		searchDistance *= kWarningDistanceRate;
	}
	//���G�͈�
	MyDrawUtils::Draw3DCircle(pos, searchDistance, 36, 0x0000ff);
	//���Ă����
	auto forward = m_model->GetDir();
	forward = forward * searchDistance;
	//����p
	auto viewDir1 = Quaternion::AngleAxis(viewingAngle / 2.0f, Vector3::Up()) * forward;
	auto viewDir2 = Quaternion::AngleAxis(-viewingAngle / 2.0f, Vector3::Up()) * forward;
	//�`��
	DrawLine3D(pos.ToDxLibVector(), (pos + forward).ToDxLibVector(), 0xff0000);
	DrawLine3D(pos.ToDxLibVector(), (pos + viewDir1).ToDxLibVector(), 0xff0000);
	DrawLine3D(pos.ToDxLibVector(), (pos + viewDir2).ToDxLibVector(), 0xff0000);
	if (m_searchPlace)
	{
		m_searchPlace->Draw();
	}
#endif
	m_model->Draw();
}

void EnemyBase::UpdateAttackCoolTime()
{
	//�U���ł��Ȃ��Ȃ�
	if (!m_canAttack)
	{
		//�U�������^����ꂽ�u�Ԃɂ����ɍU�������ƍ���̂�
		if (m_attackCoolTime < kAttackCoolTime)
		{
			m_attackCoolTime = kAttackCoolTime;
		}
		return;
	}
	m_attackCoolTime--;
	if (m_attackCoolTime < 0)
	{
		m_attackCoolTime = 0;
	}
}

void EnemyBase::SetActive(bool isActive)
{
	m_isActive = isActive;
	if (!m_isActive)
	{
		//���G
		m_hitPoints->SetIsNoDamege(true);
	}
	else
	{
		//���G����
		m_hitPoints->SetIsNoDamege(false);
	}
}

bool EnemyBase::IsStopActiveCollision(const std::weak_ptr<ActorManager> actorManager)
{
	//��~������
	bool isStop = false;
	//�v���C���[�Ƃ̋���
	auto player = actorManager.lock()->GetPlayer();
	if (player.expired())
	{
		return true;
	}
	auto dis = (player.lock()->GetPos() - m_rb->m_pos).Magnitude();
	//�v���C���[�Ƃ̋����������Ƃ��͓����蔻����~����
	if (dis > kStopCollisionDistance)
	{
		m_isThrough = true;				//�����蔻����~
		m_rb->m_isGravity = false;		//�d�͂𖳌���
		m_rb->SetVec(Vector3::Zero());	//�������~�߂�
		isStop = true;					//��~����
	}
	else
	{
		m_isThrough = false;			//�����蔻���L����
		m_rb->m_isGravity = true;		//�d�͂�L����
	}
	return isStop;
}
