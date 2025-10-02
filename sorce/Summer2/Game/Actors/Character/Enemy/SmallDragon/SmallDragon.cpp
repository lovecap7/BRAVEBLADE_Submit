#include "SmallDragon.h"
#include "SmallDragonStateBase.h"
#include "SmallDragonStateIdle.h"
#include "../../CharacterStateBase.h"
#include <memory>
#include <cassert>
#include "../../../ActorManager.h"
#include "../../Player/Player.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Input.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Collision/SphereCollider.h"
#include "../../../../../General/game.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../General/AttackPoints.h"
#include "../../../../../General/Sound/SoundManager.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../GameRule/Score.h"
#include "../../../../UI/UIManager.h"
#include "../../../SearchPlace.h"
namespace
{
	//�����蔻��
	const Vector3 kCapsuleHeight = { 0.0f,120.0f,0.0f };//�J�v�Z���̏�[
	constexpr float kCapsuleRadius = 40.0f; //�J�v�Z���̔��a
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//���O
	const std::wstring kName = L"�����h���S��";
}
SmallDragon::SmallDragon(int modelHandle, Vector3 pos) :
	EnemyBase(Shape::Capsule,EnemyGrade::Normal)
{
	//���f���̏�����
	m_model = std::make_unique<Model>(modelHandle, pos.ToDxLibVector());
	//�Փ˔���
	Vector3 endPos = pos;
	endPos += kCapsuleHeight; //�J�v�Z���̏�[
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	cap->SetRadius(kCapsuleRadius);
	cap->SetEndPos(endPos);
	//���W�b�h�{�f�B�̏�����
	m_rb->SetPos(pos);
	//�̗̓X�e�[�^�X
	m_hitPoints = std::make_shared<HitPoints>();
	//�U���X�e�[�^�X
	m_attackPoints = std::make_shared<AttackPoints>();
	//���G�ꏊ
	m_searchPlace = std::make_shared<SearchPlace>(GetPos(), 0.0f);
}

SmallDragon::~SmallDragon()
{
}

void SmallDragon::Init()
{
	//�R���C�_�u���̏�����
	AllSetting(CollisionState::Normal, Priority::Middle, GameTag::Enemy, false, false,true);
	//Physics�ɓo�^
	Collidable::Init();
	//�T�E���h
	InitSound();
	//�ҋ@��Ԃɂ���(�ŏ��̓v���C���[���ŏ�Ԃ����������邪���̂��Ƃ͊e��ԂőJ�ڂ���
	auto thisPointer = std::dynamic_pointer_cast<SmallDragon>(shared_from_this());
	m_state = std::make_shared<SmallDragonStateIdle>(thisPointer);
	//��Ԃ�ω�����
	m_state->ChangeState(m_state);
	//�G�֘A��UI�̏���
	UIManager::GetInstance().CreateEnemyUI(thisPointer);
	//���O
	m_name = kName;
}

void SmallDragon::OnCollide(const std::shared_ptr<Collidable> other)
{

}

void SmallDragon::Complete()
{
	m_rb->m_pos = m_rb->GetNextPos();//���̍��W��
	Vector3 endPos = m_rb->m_pos;
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//�J�v�Z���̈ړ�
	//���f���̍��W�X�V
	m_model->SetPos(m_rb->GetPos().ToDxLibVector());
}

void SmallDragon::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	if (!m_hitPoints->IsDead())return;//�̗͂��Ȃ��Ȃ��Ă��Ȃ��ꍇ�͖���
	//�X�R�A���Z
	score.lock()->AddKillOrItemScore(ScoreDataName::kSmallDragon);
	//�A�C�e���������_���ŗ��Ƃ�
	auto actorM = actorManager.lock();
	if (MyMath::IsRand())
	{
		actorM->CreateRandItem(m_rb->GetPos());
	}
	//���S�G�t�F�N�g
	EffekseerManager::GetInstance().CreateEffect("DeathEff", m_rb->m_pos);
	//����SE
	SoundManager::GetInstance().PlayOnceSE("Blast");
}

void SmallDragon::End()
{
	EndSound();
	m_model->End();
	Collidable::End();
}

void SmallDragon::InitSound()
{
	//VC
	m_soundHandles["Attack"] = LoadSoundMem(L"Data/Sound/VC/Enemy/SmallDragon/Attack.mp3");
	m_soundHandles["Damage"] = LoadSoundMem(L"Data/Sound/VC/Enemy/SmallDragon/Damage.mp3");
	m_soundHandles["Dead"] = LoadSoundMem(L"Data/Sound/VC/Enemy/SmallDragon/Dead.mp3");
	//���[�h�ɐ����������`�F�b�N
	for (const auto& [key, value] : m_soundHandles) {
		assert(value >= 0);
	}
}