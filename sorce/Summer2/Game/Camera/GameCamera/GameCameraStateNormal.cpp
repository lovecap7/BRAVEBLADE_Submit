#include "GameCamera.h"
#include "GameCameraStateNormal.h"
#include "GameCameraStateAreaLock.h"
#include "GameCameraStateZMove.h"
#include "GameCameraStateBossArea.h"
#include "GameCameraStateBossDeath.h"
#include "GameCameraStateClear.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/HitPoints.h"
#include "../../../General/game.h"
#include "../../Actors/Character/Player/Player.h"
#include "../../Actors/Character/Enemy/EnemyBase.h"
#include "../../Actors/Stage/BossArea.h"
#include "../../Actors/ActorManager.h"
#include "../../Actors/Stage/EventAreaBase.h"
#include <DxLib.h>

namespace
{
	//����p
	constexpr float kPerspective = 35.0f * MyMath::DEG_2_RAD;
	//�J�����p�x
	constexpr float kCameraAngleX = 30.0f * MyMath::DEG_2_RAD;
	//�^�[�Q�b�g���班������邽�߂̃I�t�Z�b�g
	constexpr float kOffsetCameraPosY = 800.0f;
	constexpr float kCameraPosZ = -900.0f;
	//�J���������炷�����̏��
	constexpr float kSlideMax = 200.0f;
	constexpr float kSlideMin = -200.0f;
	//�J�����̃X���C�h���x��
	constexpr float kSlideSpeedRate = 0.4f;
	//�ړ��J�n�t���[��
	constexpr int kMoveStartFrame = 60;
	//��ʒ����ɖ߂��������s���̂ɂ�����t���[��
	constexpr int kMoveEndFrame = 120;
	//�����ɖ߂�lerp��
	constexpr float kSlideCenterRate = 0.05f;
	//near��far
	constexpr float kNear = 300.0f;
	constexpr float kFar = 20000.0f;
}

GameCameraStateNormal::GameCameraStateNormal(std::weak_ptr<GameCamera> camera):
	GameCameraStateBase(camera),
	m_cameraSlide(0.0f),
	m_moveStartFrame(0),
	m_moveEndFrame(0)
{
	//�J���������邩�`�F�b�N
	if (m_camera.expired())return;
	auto owner = m_camera.lock();
	//�J�����̊p�x
	owner->SetDir(Matrix4x4::RotateXMat4x4(kCameraAngleX) *
		Vector3::Forward());
	//�J�����̍��W�ƒ����_
	SetCameraPositionAndTarget_UpVecY(owner->GetPos().ToDxLibVector(), owner->GetViewPos().ToDxLibVector());
	//����p
	SetupCamera_Perspective(kPerspective);
	//���s
	SetCameraNearFar(kNear, kFar);
}

void GameCameraStateNormal::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void GameCameraStateNormal::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//�{�X�����S�����ꍇ
	if (actorManager.lock()->IsBossDead())
	{
		ChangeState(std::make_shared<GameCameraStateBossDeath>(m_camera, actorManager));
		return;
	}
	//�v���C���[�����ł����ꍇ�X�V�I��
	if (actorManager.lock()->GetPlayer().expired())return;
	auto player = actorManager.lock()->GetPlayer().lock();
	auto camera = m_camera.lock();
	//�C�x���g�G���A�Ƀv���C���[���������Ȃ�
	if (!camera->GetEventArea().expired())
	{
		if (camera->GetEventArea().lock()->IsEvent())
		{
			//�������G���A�ɉ����ď�ԕω�
			auto camera = m_camera.lock();
			switch (camera->GetEventArea().lock()->GetAreaTag())
			{
			case AreaTag::AllKill:
				//�S�ŃG���A
				ChangeState(std::make_shared<GameCameraStateAreaLock>(m_camera));
				return;
				//Z���ړ��G���A
			case AreaTag::ZMove:
				ChangeState(std::make_shared<GameCameraStateZMove>(m_camera));
				return;
			case AreaTag::Boss:
				//�{�X�G���A
				ChangeState(std::make_shared<GameCameraStateBossArea>(m_camera));
				return;
			}
		}
	}
	//�ړ�
	auto playerPos = player->GetRb()->GetPos();
	//�ʒu�̍X�V
	Vector3 oldPos = camera->GetPos();
	Vector3 nextPos = camera->GetPos();
	nextPos.z = kCameraPosZ;
	nextPos.y = playerPos.y + kOffsetCameraPosY;//�v���C���[��Y���W��荂���ʒu
	//���炩��
	nextPos = Vector3::Lerp(oldPos, nextPos, 0.05f);

	//�ړ����Ă���Ȃ�
	if (player->GetRb()->GetVec().x != 0.0f && player->GetCollState() == CollisionState::Move)
	{
		//�ړ����Ă���t���[���̉��Z
		++m_moveStartFrame;
		m_moveStartFrame = MathSub::ClampInt(m_moveStartFrame, 0, kMoveStartFrame);
		//���t���[���ړ����Ă���Ȃ�X���C�h�J�n
		if (m_moveStartFrame >= kMoveStartFrame)
		{
			//�J�������v���C���[�̈ړ��ʂ̔������炢�̑傫���ňړ�
			m_cameraSlide += player->GetRb()->GetVec().x * kSlideSpeedRate;
		}
		//�ړ����Ă���̂ňړ����Ă��Ȃ��t���[���̓J�E���g���Ȃ�
		m_moveEndFrame = 0;
	}
	//�ړ����Ă��Ȃ�
	else
	{
		//�ړ������Ă��Ȃ��t���[���𐔂���
		++m_moveEndFrame;
		m_moveEndFrame = MathSub::ClampInt(m_moveEndFrame, 0, kMoveEndFrame);
		//�ړ����Ă��Ȃ��t���[�������t���[���o�߂�����
		if (m_moveEndFrame >= kMoveEndFrame)
		{
			//��ʒ����ɖ߂�
			m_cameraSlide = MathSub::Lerp(m_cameraSlide, 0.0f, kSlideCenterRate);
			//�ړ����Ă��Ȃ��̂�
			m_moveStartFrame = 0;
		}
	}
	//�͈͓��Ɏ��߂�(���� ~ ����̊�)
	m_cameraSlide = MathSub::ClampFloat(m_cameraSlide, kSlideMin, kSlideMax);
	//�v���C���[��X���W����m_cameraSlide����������
	nextPos.x = playerPos.x + m_cameraSlide;

	//���Ă������
	Vector3 dir = camera->GetDir();
	//���Ă�ʒu
	Vector3 viewPos = camera->GetViewPos();
	viewPos = nextPos + dir;
	//�ʒu�X�V
	SetCameraPositionAndTarget_UpVecY(nextPos.ToDxLibVector(), viewPos.ToDxLibVector());
	camera->SetPos(nextPos);
	camera->SetDir(dir);
	camera->SetViewPos(viewPos);
}