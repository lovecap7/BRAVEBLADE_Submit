#include "GameCameraStateBossDeath.h"
#include "GameCameraStateClear.h"
#include "GameCameraStateNormal.h"
#include "GameCameraStateBossArea.h"
#include "GameCamera.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/HitPoints.h"
#include "../../../General/game.h"
#include "../../Actors/Character/Player/Player.h"
#include "../../Actors/Character/Enemy/EnemyBase.h"
#include "../../Actors/ActorManager.h"
#include "../../../General/Collision/Physics.h"
#include "../../../General/Sound/SoundManager.h"
#include <DxLib.h>

namespace
{
	//����p
	constexpr float kPerspective = 35.0f * MyMath::DEG_2_RAD;
	//�J�����p�x
	constexpr float kCameraAngleX = 30.0f * MyMath::DEG_2_RAD;
	//lerp�̊���
	constexpr float kLerpRateX = 0.05f;
	constexpr float kLerpRateY = 0.05f;
	constexpr float kLerpRateZ = 0.01f;
	//�^�[�Q�b�g���班������邽�߂̃I�t�Z�b�g
	constexpr float kOffsetCameraPosY = 700.0f;
	constexpr float kOffsetCameraPosZ = -1300.0f;
	//�ǂ���̋���
	constexpr float kDistanceFromWall = 300.0f;
	//�q�b�g�X�g�b�v�̃t���[��
	constexpr int kHitStopFrame = 60;
	//�h���t���[��
	constexpr int kShakeFrame = 30;
	//near��far
	constexpr float kNear = 300.0f;
	constexpr float kFar = 20000.0f;
}
GameCameraStateBossDeath::GameCameraStateBossDeath(std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) :
	GameCameraStateBase(camera)
{
	//�J���������邩�`�F�b�N
	if (m_camera.expired())return;
	auto owner = m_camera.lock();
	auto boss = actorManager.lock()->GetBoss().lock();
	//�ʒu�X�V
	DxLib::SetCameraPositionAndTarget_UpVecY(owner->GetPos().ToDxLibVector(), boss->GetPos().ToDxLibVector());
	//���j�X�g�b�v
	actorManager.lock()->DelayUpdate(kHitStopFrame);
	owner->SetCameraShake(ShakePower::Highest, kShakeFrame);
	Physics::GetInstance().DelayUpdate(60);
	SoundManager::GetInstance().PlayOnceSE("LastHit");
	//���s
	SetCameraNearFar(kNear, kFar);
}

void GameCameraStateBossDeath::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void GameCameraStateBossDeath::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//�{�X�����ł�����Q�[���N���A�J������
	if (actorManager.lock()->IsBossDisappear())
	{
		ChangeState(std::make_shared<GameCameraStateClear>(m_camera, actorManager));
		return;
	}
	//�{�X�����݂��Ȃ��ꍇ�͉������Ȃ�
	if (actorManager.lock()->GetBoss().expired())return;
	auto boss = actorManager.lock()->GetBoss().lock();
	//����ł��Ȃ��Ȃ�
	if (!boss->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<GameCameraStateBossArea>(m_camera));
		return;
	}
	//�J���������邩�`�F�b�N
	if (m_camera.expired())return;
	auto camera = m_camera.lock();
	//�ʒu�X�V
	DxLib::SetCameraPositionAndTarget_UpVecY(camera->GetPos().ToDxLibVector(), boss->GetPos().ToDxLibVector());
}
