#include "Actor.h"
#include "../../General/Rigidbody.h"
#include "Character/Enemy/EnemyBase.h"
#include "../../General/HitPoints.h"
#include "Attack/AttackBase.h"
#include "SearchPlace.h"
#include "../../General/Collision/Physics.h"

Actor::Actor(Shape shape):
	Collidable(shape),
	m_isDelete(false),
	m_id(0),
	m_isSetId(false),
	m_groupTag{ "Untagged" },
	m_canAttack(true),
	m_isInGroup(false),
	m_targetData{}
{
}

void Actor::SetID(int id)
{
	//���łɓo�^���Ă�Ȃ�
	if (m_isSetId)return;
	m_id = id;
	m_isSetId = true;
}

Vector3 Actor::GetPos() const
{
	return m_rb->m_pos;
}

Vector3 Actor::GetNextPos() const
{
	return m_rb->GetNextPos();
}

void Actor::SetGroupTag(std::string& tag)
{
	m_groupTag = tag;
	//�O���[�v�ɏ������Ă��邩
	if (m_groupTag != "Untagged")
	{
		m_isInGroup = true;
	}
}

void Actor::TargetSearch(float searchDistance, float searchAngle, Vector3 targetPos)
{
	//���Z�b�g
	m_targetData.isHitTarget = false;
	//�������m�F
	auto myPos = m_rb->GetPos();
	auto dir = targetPos.XZ() - myPos.XZ();
	if (dir.Magnitude() <= searchDistance)
	{
		//����p���Ƀ^�[�Q�b�g�����邩
		auto angle = abs(Vector2::GetRad(m_model->GetDir().XZ(), dir));
		if (angle <= (searchAngle / 2.0f))
		{
			m_targetData.isHitTarget = true;
			m_targetData.targetPos = targetPos;
			m_targetData.targetDirXZ = dir.XZ().Normalize();
			m_targetData.targetDis = dir.Magnitude();
		}
	}
}

void Actor::LookAtTarget()
{
	m_model->SetDir(m_targetData.targetDirXZ.XZ());
}

void Actor::OnHitFromAttack(const std::shared_ptr<Collidable> other)
{
	if (m_hitPoints->IsDead())return;//�̗͂�0�Ȃ牽�����Ȃ�
	//�����Ɠ����^�O�Ȃ��΂�
	if (std::dynamic_pointer_cast<AttackBase>(other)->GetOwnerTag() == m_tag)return;
	auto attack = std::dynamic_pointer_cast<AttackBase>(other);
	//�U�����󂯂��̂Ńt���O�𗧂Ă�
	m_hitPoints->SetIsHit(true);
	//�U���̃_���[�W���󂯂�
	m_hitPoints->Damage(attack->GetDamage());
	//���f���̐F�ƃT�C�Y��ύX
	m_model->ModelHit();
	//�_���[�W���󂯂��甽�����邩���`�F�b�N
	if (Battle::CheckFlinchAttackAndArmor(attack->GetAttackWeight(), m_hitPoints->GetArmor()))
	{
		m_hitPoints->SetIsHitReaction(true);				//��������
		m_rb->AddVec(attack->GetKnockBackVec(m_rb->m_pos));	//�m�b�N�o�b�N
	}
}

//�T���ꏊ�����邩���`�F�b�N����֐�
bool Actor::IsHaveSearchPlace() const
{
	//���Ԃ����邩
	if (m_searchPlace)
	{
		//�͈͂�����Ȃ�true
		return m_searchPlace->GetRange() > 0.0f;
	}
	//�����܂ŗ�����
	return false;
}

void Actor::SetSearchPlaceRang(float rang)
{
	//null�`�F�b�N
	if (m_searchPlace)
	{
		m_searchPlace->SetRange(rang);
	}
}
