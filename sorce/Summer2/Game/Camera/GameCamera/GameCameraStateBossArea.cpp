#include "GameCameraStateBossArea.h"
#include "GameCameraStateAreaLock.h"
#include "GameCameraStateClear.h"
#include "GameCameraStateNormal.h"
#include "GameCameraStateBossDeath.h"
#include "GameCamera.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/game.h"
#include "../../../General/HitPoints.h"
#include "../../Actors/Character/Player/Player.h"
#include "../../Actors/Character/Enemy/EnemyBase.h"
#include "../../Actors/ActorManager.h"
#include "../../Actors/Stage/EventAreaBase.h"
#include <DxLib.h>

namespace
{
	//����p
	constexpr float kPerspective = 40.0f * MyMath::DEG_2_RAD;
	//�J�����p�x
	constexpr float kCameraAngleX = 35.0f * MyMath::DEG_2_RAD;
	//lerp�̊���
	constexpr float kLerpRateX = 0.1f;
	constexpr float kLerpRateY = 0.1f;
	constexpr float kLerpRateZ = 0.05f;
	//�^�[�Q�b�g���班������邽�߂̃I�t�Z�b�g
	constexpr float kOffsetCameraPosY = 800.0f;
	constexpr float kOffsetCameraPosZ = -1000.0f;
	//�ǂ���̋���
	constexpr float kDistanceFromWall = 300.0f;
	//�����̔��f��
	constexpr float kDistanceRate = 3.0f;
	//near��far
	constexpr float kNear = 300.0f;
	constexpr float kFar = 20000.0f;
}
GameCameraStateBossArea::GameCameraStateBossArea(std::weak_ptr<GameCamera> camera):
	GameCameraStateBase(camera),
	m_angle(kCameraAngleX)
{
	//�J���������邩�`�F�b�N
	if (m_camera.expired())return;
	auto owner = m_camera.lock();
	//���Ă�ʒu
	owner->SetViewPos(owner->GetPos() + owner->GetDir());
	//�J�����̍��W�ƒ����_
	DxLib::SetCameraPositionAndTarget_UpVecY(owner->GetPos().ToDxLibVector(), owner->GetViewPos().ToDxLibVector());
	//����p
	DxLib::SetupCamera_Perspective(kPerspective);
	//���s
	SetCameraNearFar(kNear, kFar);
}

void GameCameraStateBossArea::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void GameCameraStateBossArea::Update(const std::weak_ptr<ActorManager> actorManager)
{
	
	//�{�X�����S�����ꍇ
	if (actorManager.lock()->IsBossDead())
	{
		ChangeState(std::make_shared<GameCameraStateBossDeath>(m_camera, actorManager));
		return;
	}
	auto camera = m_camera.lock();
	//�C�x���g�����ł�����
	if (camera->GetEventArea().expired())
	{
		//�ʏ�̃J������
		ChangeState(std::make_shared<GameCameraStateNormal>(m_camera));
		return;
	}
	auto player = actorManager.lock()->GetPlayer();
	//�v���C���[�����ł����ꍇ�X�V�I��
	if (player.expired())return;
	//�G���A�̎n�_�ƏI�_
	auto area = camera->GetEventArea().lock();
	auto startPos = area->GetEventStartPos();
	auto endPos = area->GetEventEndPos();
	//�v���C���[�̍��W
	auto playerPos = player.lock()->GetPos();
	auto boss = actorManager.lock()->GetBoss();
	//�{�X�����Ȃ��Ȃ�
	if (boss.expired())return;
	//�Ԃ̈ʒu
	Vector3 center = (boss.lock()->GetPos() + playerPos) / 2.0f;
	//�v���C���[�ƃ{�X�̋���
	float distance = (boss.lock()->GetPos() - playerPos).Magnitude();
	//�ʒu�̍X�V
	Vector3 oldPos = camera->GetPos();
	Vector3 nextPos = camera->GetPos();
	//�Ԃ̋��������ăJ����������
	nextPos.y = center.y + kOffsetCameraPosY + (distance / kDistanceRate);//�v���C���[��Y���W��荂���ʒu
	//�G���A�̊O�ɃJ�������߂Â�����~�܂�
	nextPos.x = center.x;
	if (nextPos.x <= startPos.x + kDistanceFromWall)
	{
		nextPos.x = startPos.x + kDistanceFromWall;
	}
	else if (nextPos.x >= endPos.x - kDistanceFromWall)
	{
		nextPos.x = endPos.x - kDistanceFromWall;
	}
	//�Ԃ̋��������ăJ����������
	nextPos.z = center.z + kOffsetCameraPosZ - (distance / kDistanceRate);
	//���̍��W
	nextPos.x = MathSub::Lerp(oldPos.x, nextPos.x, kLerpRateX);
	nextPos.y = MathSub::Lerp(oldPos.y, nextPos.y, kLerpRateY);
	nextPos.z = MathSub::Lerp(oldPos.z, nextPos.z, kLerpRateZ);
	//���Ă������
	//�J�����̊p�x
	m_angle = kCameraAngleX + (distance / 10000.0f);
	Vector3 dir = Matrix4x4::RotateXMat4x4(m_angle) * Vector3::Forward();
	//���Ă�ʒu
	Vector3 viewPos = nextPos + dir;
	//�ʒu�X�V
	DxLib::SetCameraPositionAndTarget_UpVecY(nextPos.ToDxLibVector(), viewPos.ToDxLibVector());
	camera->SetPos(nextPos);
	camera->SetDir(dir);
	camera->SetViewPos(viewPos);
}
