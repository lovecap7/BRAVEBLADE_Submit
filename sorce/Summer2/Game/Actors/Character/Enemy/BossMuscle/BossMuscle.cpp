#include "BossMuscle.h"
#include "BossMuscleStateIdle.h"
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
#include "../../../Character/CharacterStateBase.h"
#include "../../../../UI/UIManager.h"
#include "../../../../UI/EnemyUI/BossHPUI.h"

namespace
{
	//�����蔻��
	const Vector3 kCapsuleHeight = { 0.0f,150.0f,0.0f };//�J�v�Z���̏�[
	constexpr float kCapsuleRadius = 80.0f; //�J�v�Z���̔��a
	//���f���̍�������
	constexpr float kModelHeightAdjust = -70.0f;
	//���O
	const std::wstring kName = L"�}�b�X���E����";
}

BossMuscle::BossMuscle(int modelHandle, Vector3 pos) :
	EnemyBase(Shape::Capsule, EnemyGrade::Boss)
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
	//���f���̍�������
	m_model->SetModelHeightAdjust(kModelHeightAdjust);
}

BossMuscle::~BossMuscle()
{
}

void BossMuscle::Init()
{
	//�R���C�_�u���̏�����
	AllSetting(CollisionState::Normal, Priority::High, GameTag::Enemy, false, false, true);
	//Physics�ɓo�^
	Collidable::Init();
	//�T�E���h
	InitSound();
	//�ҋ@��Ԃɂ���(�ŏ��̓v���C���[���ŏ�Ԃ����������邪���̂��Ƃ͊e��ԂőJ�ڂ���
	auto thisPointer = std::dynamic_pointer_cast<BossMuscle>(shared_from_this());
	m_state = std::make_shared<BossMuscleStateIdle>(thisPointer,false);
	//��Ԃ�ω�����
	m_state->ChangeState(m_state);
	//���O
	m_name = kName;
}

void BossMuscle::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void BossMuscle::Complete()
{
	m_rb->m_pos = m_rb->GetNextPos();//���̍��W��
	Vector3 endPos = m_rb->m_pos;
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//�J�v�Z���̈ړ�
	//���f���̍��W�X�V
	m_model->SetPos(m_rb->GetPos().ToDxLibVector());
}

void BossMuscle::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	if (!m_hitPoints->IsDead())return;//�̗͂��Ȃ��Ȃ��Ă��Ȃ��ꍇ�͖���
	//�X�R�A���Z
	score.lock()->AddKillOrItemScore(ScoreDataName::kBossMuscle);
	//���S�G�t�F�N�g
	EffekseerManager::GetInstance().CreateEffect("BossDeathEff", m_rb->m_pos);
	//����SE
	SoundManager::GetInstance().PlayOnceSE("BossBlast");
}

void BossMuscle::End()
{
	EndSound();
	m_model->End();
	Collidable::End();
}
void BossMuscle::InitSound()
{
	//VC
	m_soundHandles["Attack"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossMuscle/Attack.mp3");
	m_soundHandles["Damage"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossMuscle/Damage.mp3");
	m_soundHandles["Dead"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossMuscle/Dead.mp3");
	m_soundHandles["Angry"] = LoadSoundMem(L"Data/Sound/VC/Enemy/BossMuscle/Angry.mp3");
	//���[�h�ɐ����������`�F�b�N
	for (const auto& [key, value] : m_soundHandles) {
		assert(value >= 0);
	}
}