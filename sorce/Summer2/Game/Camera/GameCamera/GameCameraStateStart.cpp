#include "GameCameraStateStart.h"
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

GameCameraStateStart::GameCameraStateStart(std::weak_ptr<GameCamera> camera) :
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

void GameCameraStateStart::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void GameCameraStateStart::Update(const std::weak_ptr<ActorManager> actorManager)
{
	auto player = actorManager.lock()->GetPlayer();
	//�J���������邩�`�F�b�N
	if (m_camera.expired())return;
	auto camera = m_camera.lock();
	//�v���C���[�����ł����ꍇ�X�V�I��
	if (player.expired())return;
	//�v���C���[�̃X�^�[�g��Ԃ��I�������J������ʏ��Ԃ�
	if (!player.lock()->IsStartAnim())
	{
		ChangeState(std::make_shared<GameCameraStateNormal>(m_camera));
		return;
	}
	//�v���C���[
	auto playerPos = player.lock()->GetRb()->GetPos();
	//�ʒu�̍X�V
	Vector3 oldPos = camera->GetPos();
	Vector3 nextPos = camera->GetPos();
	nextPos.z = kCameraPosZ;
	nextPos.y = playerPos.y + kOffsetCameraPosY;//�v���C���[��Y���W��荂���ʒu
	nextPos.x = playerPos.x + kOffsetCameraPosX;
	//���Ă�ʒu
	Vector3 viewPos = playerPos;
	viewPos.y += kOffsetCameraViewPosY;
	//�ʒu�X�V
	SetCameraPositionAndTarget_UpVecY(nextPos.ToDxLibVector(), viewPos.ToDxLibVector());
	camera->SetPos(nextPos);
	camera->SetViewPos(viewPos);
}

