#include "PurpleDinosaur.h"
#include "PurpleDinosaurStateBase.h"
#include "PurpleDinosaurStateIdle.h"
#include "../../../ActorManager.h"
#include "../../Player/Player.h"
#include <memory>
#include <cassert>
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
	//���O
	const std::wstring kName = L"�C�m�h��";
}
PurpleDinosaur::PurpleDinosaur(int modelHandle, Vector3 pos) :
	EnemyBase(Shape::Capsule, EnemyGrade::Normal)
{
	//���f���̏�����
	m_model = std::make_unique<Model>(modelHandle, pos.ToDxLibVector());
	//�Փ˔���
	Vector3 endPos = pos;
	endPos += kCapsuleHeight; //�J�v�Z���̏�[
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	cap->SetRadius(kCapsuleRadius);
	cap->SetEndPos(endPos);
	//���f���̍�������(���a���������f���������Ă�̂�)
	m_model->SetModelHeightAdjust(-kCapsuleRadius);
	//���W�b�h�{�f�B�̏�����
	m_rb->SetPos(pos);
	//�̗̓X�e�[�^�X
	m_hitPoints = std::make_shared<HitPoints>();
	//�U���X�e�[�^�X
	m_attackPoints = std::make_shared<AttackPoints>();
	//���G�ꏊ
	m_searchPlace = std::make_shared<SearchPlace>(GetPos(), 0.0f);
}

PurpleDinosaur::~PurpleDinosaur()
{
}

void PurpleDinosaur::Init()
{
	//�R���C�_�u���̏�����
	AllSetting(CollisionState::Normal, Priority::Middle, GameTag::Enemy, false, false,true);
	//Physics�ɓo�^
	Collidable::Init();
	//�T�E���h
	InitSound();
	//�ҋ@��Ԃɂ���(�ŏ��̓v���C���[���ŏ�Ԃ����������邪���̂��Ƃ͊e��ԂőJ�ڂ���
	auto thisPointer = std::dynamic_pointer_cast<PurpleDinosaur>(shared_from_this());
	m_state = std::make_shared<PurpleDinosaurStateIdle>(thisPointer);
	//��Ԃ�ω�����
	m_state->ChangeState(m_state);
	//�G�֘A��UI�̏���
	UIManager::GetInstance().CreateEnemyUI(thisPointer);
	//���O
	m_name = kName;
}

void PurpleDinosaur::OnCollide(const std::shared_ptr<Collidable> other)
{
}



void PurpleDinosaur::Complete()
{
	m_rb->m_pos = m_rb->GetNextPos();//���̍��W��
	Vector3 endPos = m_rb->m_pos;
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//�J�v�Z���̈ړ�
	//���f���̍��W�X�V
	m_model->SetPos(m_rb->GetPos().ToDxLibVector());
}

void PurpleDinosaur::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	if (!m_hitPoints->IsDead())return;//�̗͂��Ȃ��Ȃ��Ă��Ȃ��ꍇ�͖���
	//�X�R�A���Z
	score.lock()->AddKillOrItemScore(ScoreDataName::kPurpleDinosaur);
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

void PurpleDinosaur::End()
{
	EndSound();
	m_model->End();
	Collidable::End();
}

void PurpleDinosaur::InitSound()
{
	//VC
	m_soundHandles["Attack"] = LoadSoundMem(L"Data/Sound/VC/Enemy/PurpleDinosaur/Attack.mp3");
	m_soundHandles["Damage"] = LoadSoundMem(L"Data/Sound/VC/Enemy/PurpleDinosaur/Damage.mp3");
	m_soundHandles["Dead"] = LoadSoundMem(L"Data/Sound/VC/Enemy/PurpleDinosaur/Dead.mp3");
	//���[�h�ɐ����������`�F�b�N
	for (const auto& [key, value] : m_soundHandles) {
		assert(value >= 0);
	}
}