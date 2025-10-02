#include "GameCameraStateZMove.h"
#include "GameCameraStateNormal.h"
#include "GameCameraStateAreaLock.h"
#include "GameCameraStateBossArea.h"
#include "GameCameraStateBossDeath.h"
#include "GameCameraStateClear.h"
#include "../../Actors/Stage/EventAreaBase.h"
#include "GameCamera.h"
#include "../../Actors/Character/Player/Player.h"
#include "../../Actors/ActorManager.h"
#include <DxLib.h>
namespace
{
	//����p
	constexpr float kPerspective = 35.0f * MyMath::DEG_2_RAD;
	//�J�����p�x
	constexpr float kCameraAngleX = 30.0f * MyMath::DEG_2_RAD;
	//lerp�̊���
	constexpr float kLerpRateX = 0.1f;
	constexpr float kLerpRateY = 0.1f;
	constexpr float kLerpRateZ = 0.1f;
	//�^�[�Q�b�g���班������邽�߂̃I�t�Z�b�g
	constexpr float kOffsetCameraPosY = 900.0f;
	constexpr float kOffsetCameraPosZ = -1400.0f;
	//near��far
	constexpr float kNear = 300.0f;
	constexpr float kFar = 35000.0f;
}

GameCameraStateZMove::GameCameraStateZMove(std::weak_ptr<GameCamera> camera):
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
	DxLib::SetupCamera_Perspective(kPerspective);
	//���s
	SetCameraNearFar(kNear, kFar);
}

void GameCameraStateZMove::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void GameCameraStateZMove::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//�{�X�����S�����ꍇ
	if (actorManager.lock()->IsBossDead())
	{
		ChangeState(std::make_shared<GameCameraStateBossDeath>(m_camera, actorManager));
		return;
	}
	//�J���������邩�`�F�b�N
	if (m_camera.expired())return;
	auto camera = m_camera.lock();

	if (!camera->GetEventArea().expired())
	{
		//�C�x���g�I��
		if (!camera->GetEventArea().lock()->IsEvent())
		{
			//�ʏ�̃J������
			ChangeState(std::make_shared<GameCameraStateNormal>(m_camera));
			return;
		}
		//�C�x���g�G���A�̃^�O�ɂ���ď�Ԃ�ς���
		switch (camera->GetEventArea().lock()->GetAreaTag())
		{
		case AreaTag::AllKill:
			//�S�ŃG���A
			ChangeState(std::make_shared<GameCameraStateAreaLock>(m_camera));
			return;
		case AreaTag::Boss:
			//�{�X�G���A
			ChangeState(std::make_shared<GameCameraStateBossArea>(m_camera));
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
		//�ʒu�̍X�V
		Vector3 oldPos = camera->GetPos();
		Vector3 nextPos = camera->GetPos();
		nextPos.y = playerPos.y + kOffsetCameraPosY;//�v���C���[��Y���W��荂���ʒu
		//�G���A�̊O�ɃJ�������߂Â�����~�܂�
		nextPos.x = playerPos.x;
		nextPos.z = playerPos.z + kOffsetCameraPosZ;
		//���̍��W
		nextPos.x = MathSub::Lerp(oldPos.x, nextPos.x, kLerpRateX);
		nextPos.y = MathSub::Lerp(oldPos.y, nextPos.y, kLerpRateY);
		nextPos.z = MathSub::Lerp(oldPos.z, nextPos.z, kLerpRateZ);
		//���Ă������
		Vector3 dir = camera->GetDir();
		//���Ă�ʒu
		Vector3 viewPos = nextPos + dir;
		//�ʒu�X�V
		DxLib::SetCameraPositionAndTarget_UpVecY(nextPos.ToDxLibVector(), viewPos.ToDxLibVector());
		camera->SetPos(nextPos);
		camera->SetDir(dir);
		camera->SetViewPos(viewPos);
	}
	//�C�x���g�����ł�����
	else
	{
		//�ʏ�̃J������
		ChangeState(std::make_shared<GameCameraStateNormal>(m_camera));
		return;
	}
}
