#include "HomingBullet.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../Character/CharacterBase.h"
#include "../../../General/HitPoints.h"
#include "../../../General/Sound/SoundManager.h"
#include "../../Camera/GameCamera/GameCamera.h"
#include "../Character/Player/Player.h"
#include "../Character/Player/UltGage.h"
#include "../ActorManager.h"

namespace
{
	//�^�[�Q�b�g�̏������ڕW�n�_�Ƃ���
	constexpr float kTargetHeight = 100.0f;
}

HomingBullet::HomingBullet(std::weak_ptr<Actor> owner):
	SphereAttackBase(owner),
	m_speed(0.0f),
	m_trackingRate(0.0f)
{
	//SE
	SoundManager::GetInstance().PlayOnceSE("Shot");
}

void HomingBullet::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�U�������������Ȃ�
	if (m_isHit)
	{
		//SE
		if (MyMath::IsRand())
		{
			SoundManager::GetInstance().PlayOnceSE("Hit1");
		}
		else
		{
			SoundManager::GetInstance().PlayOnceSE("Hit2");
		}
		//�q�b�g�X�g�b�v
		actorManager.lock()->HitStop(m_shakePower, m_hitStopFrame);
		//�폜
		m_isDelete = true;
		return;
	}
	//���ʂ̏���������
	AttackBase::Update(actorManager);
	//���񂾂�^�[�Q�b�g�Ɍ�����ς���
	if (!m_target.expired())
	{
		//���݂̃x�N�g��
		auto nowVec = m_rb->m_vec;
		if (nowVec.SqMagnitude() > 0.0f)
		{
			nowVec = nowVec.Normalize();
		}
		//�^�[�Q�b�g�ւ̌���
		auto targetPos = m_target.lock()->GetPos();
		targetPos.y += kTargetHeight;
		auto targetDir = targetPos - m_rb->m_pos;
		if (targetDir.SqMagnitude() > 0.0f)
		{
			targetDir = targetDir.Normalize();
		}
		//�^�[�Q�b�g�֏����X����
		nowVec = Vector3::Lerp(nowVec, targetDir, m_trackingRate);
		if (nowVec.SqMagnitude() > 0.0f)
		{
			nowVec = nowVec.Normalize();
		}
		m_rb->m_vec = nowVec * m_speed;
	}

	//�ړ�
	m_rb->m_pos = m_rb->GetNextPos();
}


void HomingBullet::OnCollide(const std::shared_ptr<Collidable> other)
{
	if (m_owner.expired())return;
	auto ownerColl = m_owner.lock();
	auto otherColl = other;

	//�����Ɠ�����ނȂ疳��
	if (otherColl->GetGameTag() == ownerColl->GetGameTag())return;

	//�v���C���[���G�Ȃ�
	if (otherColl->GetGameTag() == GameTag::Player ||
		otherColl->GetGameTag() == GameTag::Enemy)
	{
		if (std::dynamic_pointer_cast<CharacterBase>(otherColl)->GetHitPoints().lock()->IsNoDamege())
		{
			//�_���[�W���󂯂Ȃ���ԂȂ疳��
			return;
		}
	}
	else
	{
		//����ȊO��Actor�͖���
		return;
	}
	//�v���C���[�̍U���̏ꍇ�K�E�Z�Q�[�W�����Z����
	if (ownerColl->GetGameTag() == GameTag::Player)
	{
		std::dynamic_pointer_cast<Player>(ownerColl)->GetUltGage().lock()->AddPedingUltGage();//�\�񂳂�Ă������Z�Q�[�W�ʂ𔽉f
	}
	//�U�����󂯂��Ƃ��̏���
	std::dynamic_pointer_cast<CharacterBase>(otherColl)->OnHitFromAttack(shared_from_this());
	//�q�b�g�G�t�F�N�g
	auto HitPos = m_rb->m_pos;
	EffekseerManager::GetInstance().CreateEffect("ImpactHitEff", HitPos);
	//�U���𓖂Ă��̂�
	m_isHit = true;
}


void HomingBullet::Draw() const
{
	auto coll = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
	DrawSphere3D(m_rb->m_pos.ToDxLibVector(),
		coll->GetRadius(), 16, 0xff0000, 0xffffff, true);
}


void HomingBullet::SetVec(Vector3 vec)
{
	//�ړ���
	m_rb->m_vec = vec;
}

void HomingBullet::SetTarget(std::weak_ptr<Actor> target)
{
	m_target = target;
}
