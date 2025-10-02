#include "GameCameraStateBossStart.h"
#include "GameCamera.h"
#include "GameCameraStateNormal.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/HitPoints.h"
#include "../../../General/game.h"
#include "../../Actors/Character/Player/Player.h"
#include "../../Actors/Character/Enemy/EnemyBase.h"
#include "../../Actors/Stage/BossArea.h"
#include "../../Actors/ActorManager.h"
#include "../../Actors/Stage/EventAreaBase.h"
#include "../../Actors/Character/Enemy/BossKing/BossKing.h"
#include <DxLib.h>

namespace
{
	//����p
	constexpr float kPerspective = 35.0f * MyMath::DEG_2_RAD;
	//�J�����p�x
	constexpr float kCameraAngleX = 0.0f * MyMath::DEG_2_RAD;
	//lerp�̊���
	constexpr float kLerpRate = 0.07f;
	//�^�[�Q�b�g���班������邽�߂̃I�t�Z�b�g
	constexpr float kOffsetCameraPosY = 120.0f;
	constexpr float kOffsetCameraPosX = 100.0f;
	constexpr float kCameraPosZ = -100.0f;
	//���Ă�ʒu
	constexpr float kOffsetCameraViewPosY = 100.0f;
	//near��far
	constexpr float kNear = 300.0f;
	constexpr float kFar = 20000.0f;
}

GameCameraStateBossStart::GameCameraStateBossStart(std::weak_ptr<GameCamera> camera) :
	GameCameraStateBase(camera)
{
	//�J���������邩�`�F�b�N
	if (m_camera.expired())return;
	auto owner = m_camera.lock();
	//�J�����̊p�x
	owner->SetDir(Matrix4x4::RotateXMat4x4(kCameraAngleX) *
		Vector3::Forward());
	//���Ă�ʒu
	owner->SetViewPos(owner->GetPos() + owner->GetDir());
	//�J�����̍��W�ƒ����_
	DxLib::SetCameraPositionAndTarget_UpVecY(owner->GetPos().ToDxLibVector(), owner->GetViewPos().ToDxLibVector());
	//����p
	SetupCamera_Perspective(kPerspective);
	//���s
	SetCameraNearFar(kNear, kFar);
}

void GameCameraStateBossStart::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void GameCameraStateBossStart::Update(const std::weak_ptr<ActorManager> actorManager)
{
	auto boss = actorManager.lock()->GetBoss();
	//�J���������邩�`�F�b�N
	if (m_camera.expired())return;
	auto camera = m_camera.lock();
	//�v���C���[�����ł����ꍇ�X�V�I��
	if (boss.expired())return;
	auto king = std::dynamic_pointer_cast<BossKing>(boss.lock());
	//�v���C���[�̃X�^�[�g��Ԃ��I�������J������ʏ��Ԃ�
	if (!king->IsStartAnim())
	{
		ChangeState(std::make_shared<GameCameraStateNormal>(m_camera));
		return;
	}
	//�{�X
	auto bossPos = king->GetRb()->GetPos();
	//�ʒu�̍X�V
	Vector3 oldPos = camera->GetPos();
	Vector3 nextPos = camera->GetPos();
	nextPos.z = kCameraPosZ;
	nextPos.y = bossPos.y + kOffsetCameraPosY;
	nextPos.x = bossPos.x + kOffsetCameraPosX;
	//���Ă�ʒu
	Vector3 viewPos = bossPos;
	viewPos.y += kOffsetCameraViewPosY;
	//�ʒu�X�V
	SetCameraPositionAndTarget_UpVecY(nextPos.ToDxLibVector(), viewPos.ToDxLibVector());
	camera->SetPos(nextPos);
	camera->SetViewPos(viewPos);
}

