#include "CharacterBase.h"
#include "../../../General/Model.h"
#include "../../../General/Sound/SoundManager.h"
#include "../../../General/HitPoints.h"
#include "../../../General/AttackPoints.h"

CharacterBase::CharacterBase(Shape shape) :
	Actor(shape)
{
}

std::weak_ptr<SE>  CharacterBase::CharacterOnceSE(std::string name)
{
	return SoundManager::GetInstance().PlayOnceSE(m_soundHandles.at(name));
}

std::weak_ptr<SE>  CharacterBase::CharacterLoopSE(std::string name)
{
	return SoundManager::GetInstance().PlayLoopSE(m_soundHandles.at(name));
}

std::weak_ptr<Voice> CharacterBase::CharacterVC(std::string name)
{
	return SoundManager::GetInstance().PlayVC(m_soundHandles.at(name));
}

void CharacterBase::SetCharaData(ActorData data)
{
	//�󂯎�����f�[�^�����Ă���
	if (m_model)
	{
		//�傫��
		m_model->SetScale(data.scale);
		//��]
		m_model->SetRot(data.rot);
		//���񑬓x
		m_model->SetRotSpeed(data.modelRotateSpeed);
	}
	//�̗�
	if (m_hitPoints)
	{
		m_hitPoints->SetHp(data.hp);
		m_hitPoints->SetArmor(data.armor);
	}
	//�U����
	if (m_attackPoints)
	{
		m_attackPoints->SetAttackPower(data.attackPower);
	}
	//�O���[�v
	SetGroupTag(data.gropeTag);
	//�T���ꏊ�͈̔�
	SetSearchPlaceRang(data.searchPlaceRang);
	//����p
	m_viewingAngle = data.viewingAngle;
	//���G����
	m_searchDistance = data.searchDistance;
}

void CharacterBase::EndSound()
{
	//�폜
	for (const auto& [key, value] : m_soundHandles) {
		DeleteSoundMem(value);
	}
}