#include "Player.h"
#include "PlayerStateBase.h"
#include "PlayerStateStart.h"
#include "PlayerStateRun.h"
#include "PlayerStateWin.h"
#include "../../Attack/AttackBase.h"
#include "../../Character/Enemy/EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/AttackPoints.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Collision/PolygonCollider.h"
#include "../../../../General/Collision/SphereCollider.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../General/Effect/TrackActorEffect.h"
#include "../../../../General/Sound/SoundManager.h"
#include "../../ActorManager.h"
#include "../../../../Game/Camera/GameCamera/GameCamera.h"
#include "UltGage.h"
#include "../../../UI/UIManager.h"
#include <DxLib.h>
#include <cmath>
#include <cassert>

namespace
{
	//�����蔻��
	const Vector3 kCapsuleHeight = { 0.0f,150.0f,0.0f };	//�J�v�Z���̏�[
	constexpr float kCapsuleRadius = 20.0f;					//�J�v�Z���̔��a
	//�K�E�Z�Q�[�W�̍ő�l
	constexpr int kMaxUltGage = 100;
	//���G�̊���������߂�ۂ̃��f���̌����Ɠ��͂̌����̊���
	constexpr float kTargetSearchDirRate = 0.2f;
	//�_�b�V��������ԉ���
	constexpr int kCancelRunFrame = 5;
	//���������Ɣ��肷��Y���W
	constexpr float kStageFallY = -500.0f;
}

Player::Player(int modelHandle, Position3 firstPos) :
	CharacterBase(Shape::Capsule),
	m_stickVec(0.0f,0.0f),
	m_isRunKeep(false),
	m_cancelRunFrame(0),
	m_initPos{},
	m_isDead(false)
{
	//���W
	m_rb->m_pos = firstPos;
	//�J�v�Z���̏I�_�̐ݒ�
	Vector3 endPos = firstPos;
	endPos += kCapsuleHeight; //�J�v�Z���̏�[
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	cap->SetEndPos(endPos);
	cap->SetRadius(kCapsuleRadius);
	//���f��
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//���f���̍�������(���a���������f���������Ă�̂�)
	m_model->SetModelHeightAdjust(-kCapsuleRadius);
	//�K�E�Z�Q�[�W
	m_ultGage = std::make_shared<UltGage>(kMaxUltGage);
	//�̗̓X�e�[�^�X
	m_hitPoints = std::make_shared<HitPoints>();
	//�U���X�e�[�^�X
	m_attackPoints = std::make_shared<AttackPoints>();
}

Player::~Player()
{
}

void Player::Init()
{
	//�R���X�g���N�^�őS����������������������
	//shared_from_this()���g���ꍇ�R���X�g���N�^���_�ł�
	//���������m�肵�Ă��Ȃ��̂Ŏg�����Ƃ��ł��Ȃ�
	//�΍�Ƃ���Init���g��

	//�R���C�_�u���̏�����
	AllSetting(CollisionState::Normal, Priority::Middle, GameTag::Player, false, false,true);
	//Physics�ɓo�^
	Collidable::Init();
	//��������
	InitSound();
	//�ҋ@��Ԃɂ���(�ŏ��̓v���C���[���ŏ�Ԃ����������邪���̂��Ƃ͊e��ԂőJ�ڂ���
	auto thisPointer = std::dynamic_pointer_cast<Player>(shared_from_this());
	m_state = std::make_shared<PlayerStateStart>(thisPointer);
	//��Ԃ�ω�����
	m_state->ChangeState(m_state);
	//���������W
	m_initPos = m_rb->m_pos;
	//�v���C���[�֘A��UI�̏���
	UIManager::GetInstance().CreatePlayerUI(thisPointer);
}


void Player::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//���͂̎擾
	auto& input = Input::GetInstance();

	//�v���C���[�̈ړ��⃂�f���̌������X�V���邽�߂�
	//���̓x�N�g�����X�V
	UpdatePlayerStickVec(input);
	//�^�[�Q�b�g�𔭌��ł��������`�F�b�N
	auto target = actorManager.lock()->GetNearestEnemy();
	if (!target.expired())
	{
		TargetSearch(m_searchDistance, m_viewingAngle, target.lock()->GetPos());
	}
	//������p�����邩
	CheckRunKeep();
	//�K�E�Q�[�W���ő�̎��G�t�F�N�g������
	CheckUltMax();
	//��Ԃɍ��킹���X�V
	m_state->Update(camera,actorManager);
	//��Ԃ��ς���������`�F�b�N
	if (m_state != m_state->GetNextState())
	{
		//��Ԃ�ω�����
		m_state = m_state->GetNextState();
		m_state->Init();
	}
	//�A�j���[�V�����̍X�V
	m_model->Update();
	//�̗̓X�e�[�^�X�̍X�V
	m_hitPoints->Update();
	//�U���X�e�[�^�X�̍X�V
	m_attackPoints->Update();

#if _DEBUG
	//�Q�[�W�}�b�N�X
	if (input.IsTrigger("Max"))
	{
		m_ultGage->AddUltGage(10000);
		m_hitPoints->Heal(10000);
	}
	//�{�X�̓���ɔ��
	if (input.IsPress("BossArea"))
	{
		auto bossPos = actorManager.lock()->GetBoss().lock()->GetPos();
		m_rb->m_pos.y = bossPos.y + 100.0f;
		m_rb->m_pos.x = bossPos.x;
		m_rb->m_vec = Vector3::Zero();
	}
	//�̗͂�0��
	if (input.IsPress("PlayerDead"))
	{
		m_hitPoints->Damage(999999);
	}
#endif
}

void Player::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void Player::Draw() const
{
#if _DEBUG
	//�Փ˔���
	DrawCapsule3D(
		m_rb->m_pos.ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		false//�n�ʂɂ���Ɠh��Ԃ����
	);
	////�T���͈�
	//DrawSphere3D(m_rb->m_pos.ToDxLibVector(), kSearchDistance, 4, 0x0000ff, 0x0000ff, false);
	////���Ă����
	//auto forward = m_model->GetDir();
	//forward = forward * kSearchDistance;
	////����p
	//auto viewDir1 = Quaternion::AngleAxis(kSearchAngle / 2.0f, Vector3::Up()) * forward;
	//auto viewDir2 = Quaternion::AngleAxis(-kSearchAngle / 2.0f, Vector3::Up()) * forward;
	////�`��
	//DrawLine3D(m_rb->m_pos.ToDxLibVector(), (m_rb->m_pos + forward).ToDxLibVector(), 0xff0000);
	//DrawLine3D(m_rb->m_pos.ToDxLibVector(), (m_rb->m_pos + viewDir1).ToDxLibVector(), 0xff00ff);
	//DrawLine3D(m_rb->m_pos.ToDxLibVector(), (m_rb->m_pos + viewDir2).ToDxLibVector(), 0xff00ff);
	////�^�[�Q�b�g�Ƃ̋���
	//if (m_targetData.isHitTarget)
	//{
	//	printf("�^�[�Q�b�g�Ƃ̋��� : %f\n", m_targetData.targetDis);
	//	DrawLine3D(m_rb->m_pos.ToDxLibVector(), m_targetData.targetPos.ToDxLibVector(), 0x0000ff);
	//}
#endif
	m_model->Draw();
}

void Player::Complete()
{
	//���������ۂ̏���
	if (m_rb->m_pos.y < kStageFallY)
	{
		m_rb->m_pos = m_initPos;
		m_rb->m_vec = Vector3::Zero();
	}

	m_rb->m_pos += m_rb->m_vec;//���̍��W��
	Vector3 endPos = m_rb->m_pos;
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//�J�v�Z���̈ړ�
	//���f���̍��W�X�V
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
}

void Player::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	//���S�G�t�F�N�g
	EffekseerManager::GetInstance().CreateEffect("DeathEff", m_rb->m_pos);
}

void Player::End()
{
	m_model->End();
	EndSound();
	Collidable::End();
}
void Player::Revival()
{
	m_hitPoints->Heal(999999);
	m_isDead = false;
}

bool Player::IsStartAnim()
{
	//�X�^�[�g��ԈȊO��false
	if (std::dynamic_pointer_cast<PlayerStateStart>(m_state) == nullptr)
	{
		return false;
	}
	return true;
}

bool Player::IsClearAnim()
{
	//�N���A��ԈȊO��false
	if (std::dynamic_pointer_cast<PlayerStateWin>(m_state) == nullptr)
	{
		return false;
	}
	return true;
}

bool Player::IsFinishClearAnim()
{
	//������ԈȊO��false
	if (std::dynamic_pointer_cast<PlayerStateWin>(m_state) == nullptr)
	{
		return false;
	}
	//������Ԃ̎��ɃA�j���[�V�������I��������true
	return m_model->IsFinishAnim();
}

void Player::TargetSearch(float searchDistance, float searchAngle, Vector3 targetPos)
{
	//���Z�b�g
	m_targetData.isHitTarget = false;
	//�������m�F
	auto toTarget = targetPos.XZ() - m_rb->GetPos().XZ();
	//����
	auto dir = toTarget;
	//���͂�����Ȃ�
	if (m_stickVec.Magnitude())
	{
		//���͂Ƃ̊Ԃ̃x�N�g��������G
		dir = Vector2::Lerp(m_model->GetDir().XZ(), m_stickVec, kTargetSearchDirRate);
	}
	if (toTarget.Magnitude() <= searchDistance)
	{
		//����p���Ƀ^�[�Q�b�g�����邩
		auto angle = abs(Vector2::GetRad(m_model->GetDir().XZ(), dir));
		if (angle <= (searchAngle))
		{
			m_targetData.isHitTarget = true;
			m_targetData.targetPos = targetPos;
			m_targetData.targetDirXZ = dir.XZ().Normalize();
			m_targetData.targetDis = toTarget.Magnitude();
		}
	}
}

void Player::UpdatePlayerStickVec(Input& input)
{
	//�X�e�B�b�N�̌���������(���f����Z���������΂Ȃ̂�)
	m_stickVec.x = static_cast<float>(input.GetStickInfo().leftStickX);
	m_stickVec.y = -static_cast<float>(input.GetStickInfo().leftStickY);
}

void Player::CheckRunKeep()
{
	//�_�b�V����Ԃ���Ȃ��Ƃ�
	if (std::dynamic_pointer_cast<PlayerStateRun>(m_state) == nullptr)
	{
		++m_cancelRunFrame;
		//����
		if (m_cancelRunFrame > kCancelRunFrame)
		{
			m_isRunKeep = false;
			m_cancelRunFrame = 0;
		}
	}
	else
	{
		m_cancelRunFrame = 0;
	}
}

void Player::CheckUltMax()
{
	//�G�t�F�N�g�������ĂȂ��ĕK�E�Q�[�W�ő�Ȃ�G�t�F�N�g������
	if (m_ultMaxEff.expired() && m_ultGage->IsMaxUlt())
	{
		m_ultMaxEff = EffekseerManager::GetInstance().CreateTrackActorEffect("UltGageMaxEff", std::dynamic_pointer_cast<Actor>(shared_from_this()));
	}
	else if (!m_ultMaxEff.expired() && !m_ultGage->IsMaxUlt())
	{
		m_ultMaxEff.lock()->Delete();
	}
}

void Player::InitSound()
{
	//�v���C���[SE
	m_soundHandles["NA"] = LoadSoundMem(L"Data/Sound/SE/Player/NA.mp3");
	m_soundHandles["CA"] = LoadSoundMem(L"Data/Sound/SE/Player/CA.mp3");
	m_soundHandles["UltCharge"] = LoadSoundMem(L"Data/Sound/SE/Player/UltCharge.mp3");
	m_soundHandles["UltLaser"] = LoadSoundMem(L"Data/Sound/SE/Player/UltLaser.mp3");
	m_soundHandles["UltShot"] = LoadSoundMem(L"Data/Sound/SE/Player/UltShot.mp3");
	m_soundHandles["CARankUp"] = LoadSoundMem(L"Data/Sound/SE/Player/CARankUp.mp3");
	m_soundHandles["CARankMax"] = LoadSoundMem(L"Data/Sound/SE/Player/CARankMax.mp3");
	m_soundHandles["CACharge"] = LoadSoundMem(L"Data/Sound/SE/Player/CACharge.mp3");
	//VC
	m_soundHandles["Attack1"] = LoadSoundMem(L"Data/Sound/VC/Player/Attack1.mp3");
	m_soundHandles["Attack2"] = LoadSoundMem(L"Data/Sound/VC/Player/Attack2.mp3");
	m_soundHandles["Attack3"] = LoadSoundMem(L"Data/Sound/VC/Player/Attack3.mp3");
	m_soundHandles["Damage1"] = LoadSoundMem(L"Data/Sound/VC/Player/Damage1.mp3");
	m_soundHandles["Damage2"] = LoadSoundMem(L"Data/Sound/VC/Player/Damage2.mp3");
	m_soundHandles["Damage3"] = LoadSoundMem(L"Data/Sound/VC/Player/Damage3.wav");
	m_soundHandles["Ult"] = LoadSoundMem(L"Data/Sound/VC/Player/Ult.mp3");
	m_soundHandles["Dead"] = LoadSoundMem(L"Data/Sound/VC/Player/Dead.wav");
	m_soundHandles["Result"] = LoadSoundMem(L"Data/Sound/VC/Player/Result.mp3");
	m_soundHandles["Rolling1"] = LoadSoundMem(L"Data/Sound/VC/Player/Rolling1.wav");
	m_soundHandles["Rolling2"] = LoadSoundMem(L"Data/Sound/VC/Player/Rolling2.wav");
	m_soundHandles["Start"] = LoadSoundMem(L"Data/Sound/VC/Player/Start.mp3");
	m_soundHandles["Jump"] = LoadSoundMem(L"Data/Sound/VC/Player/Jump.mp3");
	//���[�h�ɐ����������`�F�b�N
	for (const auto& [key, value] : m_soundHandles) {
		assert(value >= 0);
	}
}