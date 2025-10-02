#include "GameManager.h"
#include "../General/Input.h"
#include <DxLib.h>
#include "../General/game.h"
#include "Actors/ActorManager.h"
#include "Actors/Character/Player/Player.h"
#include "Actors/Character/Enemy/EnemyBase.h""
#include "UI/UIManager.h"
#include "../General/Collision/Physics.h"
#include "../Game/Camera/GameCamera/GameCamera.h"
#include "../Game/GameRule/Score.h"
#include "../Game/GameRule/Timer.h"
#include "../Game/UI/GameScoreUI.h"
#include "../Game/UI/TimerUI.h"
#include "../Game/UI/TutorialUI.h"
#include "../General/Effect/EffekseerManager.h"
#include "TutorialDirecter.h"
#include "../SaveData/SaveDataManager.h"
#include <cassert>

namespace
{
	//�V���h�E�}�b�v�̕`��T�C�Y
	constexpr int kShadowMapWidth = 8192;
	constexpr int kShadowMapHeight = 8192;
	//���C�g�̌���
	const VECTOR kLightDir = { 0.1f, -1.0f, 0.5f };
	//const VECTOR kLightDir2 = { 0.5f, 1.0f, 0.0f };
	//�V���h�E�}�b�v�͈̔�
	constexpr float kShadowMapHorizon = 5000.0f;
	constexpr float kShadowMapVerticalMin = -1.0f;
	constexpr float kShadowMapVerticalMax = 1000.0f;
}

GameManager::GameManager():
	m_isGameover(false),
	m_isGameClear(false),
	m_isResult(false),
	m_shadowMapHandle(-1)
{
	//�J�����̏�����
	m_camera = std::make_shared<GameCamera>();
	//�A�N�^�[�}�l�[�W���[
	m_actorManager = std::make_shared<ActorManager>(m_camera);
	//�^�C�}�[
	m_timer = std::make_shared<Timer>();
}


GameManager::~GameManager()
{
}

void GameManager::Init(Stage::StageIndex index)
{
	//�^�C�}�[�̏�����
	m_timer->Init();
	//�A�N�^�[�}�l�[�W���[�̏�����
	m_actorManager->Init(index);
	//�J�����̏�����
	m_camera->Init(index);
	auto& saveDataManager = SaveDataManager::GetInstance();
	auto score = saveDataManager.GetScore();
	//�X�R�A�̏�����
	if (!score.expired())score.lock()->Init();
	//UI�쐬
	UIManager::GetInstance().CreateGameScoreUI(score);
	UIManager::GetInstance().CreateTimerUI(m_timer);

	//�X�e�[�W1�Ȃ�`���[�g���A��(�����N���A�Ȃ�)
	if (index == Stage::StageIndex::Stage1 && !saveDataManager.IsClearStage(Stage::StageIndex::Stage1))
	{
		m_tutorialDirecter = std::make_shared<TutorialDirecter>();
	}

	//���C�g
	InitLight();
	//�V���h�E�}�b�v�̏���
	InitShadow();
	
}

void GameManager::Update()
{
	auto& saveDataManager = SaveDataManager::GetInstance();
	//�f�o�b�O�ňꎞ��~����ĂȂ��Ȃ�
	auto& input = Input::GetInstance();
#if _DEBUG
	if (input.IsTrigger("StopUpdate"))
	{
		m_isUpdateStop = !m_isUpdateStop;
	}
	if (m_isUpdateStop)
	{
		//�X�V���~�߂�
		Physics::GetInstance().StopUpdate();
	}
	if (!m_isUpdateStop || input.IsTrigger("OneFrame"))
#endif
	{
		//�X�V���ĊJ
		Physics::GetInstance().StartUpdate();
		//�A�N�^�[�̍X�V
		m_actorManager->Update(saveDataManager.GetScore());
		//�Q�[���J�n���̏���
		UpdateGameStart();
		//�`���[�g���A��
		if (m_tutorialDirecter != nullptr)
		{
			m_tutorialDirecter->Update(m_actorManager);
		}
		//�v���C���[
		if (m_actorManager->GetPlayer().expired())return;
		auto player = m_actorManager->GetPlayer().lock();
		//�{�X��|�����Ƃ�
		if (m_actorManager->IsBossDead())
		{
			//�^�C�}�[���~�߂�
			m_timer->StopUpdate();
			//UI�̕`����~�߂�
			UIManager::GetInstance().StopDraw();
			//�N���A���Ă��Ȃ��Ȃ�
			if (!m_isGameClear)input.StopUpdate();//���͂��~�߂�
			else input.StartUpdate();//���͂��J�n
			//�v���C���[�̏����A�j���[�V�����������Ƃ�
			if (player->IsClearAnim())
			{
				//�N���A
				m_isGameClear = true;
			}
			//�v���C���[�̏����A�j���[�V�������I��������
			if (player->IsFinishClearAnim() && m_isGameClear)
			{
				//�^�C�}�[���X�R�A�ɉ��Z
				if (saveDataManager.GetScore().expired())return;
				saveDataManager.GetScore().lock()->AddTimeScore(m_timer->GetTime());
				//���U���g��
				m_isResult = true;
			}
		}
		//�v���C���[�����S�����ۂ̏���
		else if (player->IsDead() && !m_isGameover)
		{
			//UI�̔�\��
			UIManager::GetInstance().AllStopDraw();
			//�Q�[���I�[�o�[
			m_isGameover = true;
		}
		//�^�C�}�[
		m_timer->Update();
		//�J�����̍X�V
		m_camera->Update(m_actorManager);
	}
	//�V���h�E�}�b�v�ɕ`�悷��͈͂�ݒ�
	//�J�����̎��͂̂�
	UpdateShadowDrawArea();
}

void GameManager::Draw() const
{
#if _DEBUG
	DrawString(0, 0, L"Stage1 Scene", 0xffffff);
	DrawString(0, 16, L"[D]�L�[�� Debug Scene", 0xffffff);

	//for (int z = -500; z <= 500; z += 100)
	//{
	//	DrawLine3D(VGet(-500, 0, z), VGet(500, 0, z), 0xff0000);
	//}
	//for (int x = -500; x <= 500; x += 100)
	//{
	//	DrawLine3D(VGet(x, 0, -500), VGet(x, 0, 500), 0x0000ff);
	//}
	//VECTOR screenPos = ConvWorldPosToScreenPos(VGet(500, 0, 0));
	//DrawString(screenPos.x, screenPos.y, L"X+", 0xffffff);
	//screenPos = ConvWorldPosToScreenPos(VGet(-500, 0, 0));
	//DrawString(screenPos.x, screenPos.y, L"X-", 0xffffff);
	//screenPos = ConvWorldPosToScreenPos(VGet(0, 0, 500));
	//DrawString(screenPos.x, screenPos.y, L"Z+", 0xffffff);
	//screenPos = ConvWorldPosToScreenPos(VGet(0, 0, -500));
	//DrawString(screenPos.x, screenPos.y, L"Z-", 0xffffff);
#endif
	if (m_shadowMapHandle >= 0)
	{
		//�V���h�E�}�b�v���z�肷�郉�C�g�̕������Z�b�g
		SetShadowMapLightDirection(m_shadowMapHandle, kLightDir);
		//�V���h�E�}�b�v�ւ̕`��̏���
		ShadowMap_DrawSetup(m_shadowMapHandle);
		//�V���h�E�}�b�v�ւ̃A�N�^�[�̕`��
		m_actorManager->Draw();
		//�V���h�E�}�b�v�ւ̕`����I��
		ShadowMap_DrawEnd();
		//�`��Ɏg�p����V���h�E�}�b�v��ݒ�
		SetUseShadowMap(0, m_shadowMapHandle);
		//�A�N�^�[�̕`��
		m_actorManager->Draw();
		//�`��Ɏg�p����V���h�E�}�b�v�̐ݒ������
		SetUseShadowMap(0, -1);
	}
}

void GameManager::End()
{
	//�A�N�^�[�}�l�[�W���[�̏I��
	m_actorManager->End();
	//UI�}�l�[�W���[�̃��Z�b�g
	UIManager::GetInstance().Reset();
	//�G�t�F�N�g�����Z�b�g
	EffekseerManager::GetInstance().Reset();
	//���C�g�̍폜
	for (auto& handle : m_lightHandles)
	{
		DeleteLightHandle(handle);
	}
	//�V���h�E�}�b�v�̍폜
	DeleteShadowMap(m_shadowMapHandle);
	//�`���[�g���A���I������
	if (m_tutorialDirecter != nullptr)
	{
		m_tutorialDirecter->End();
	}
}

void GameManager::Restart(Stage::StageIndex index)
{
	//�^�C�}�[�̏�����
	m_timer->Init();
	//UI�}�l�[�W���[�̃��Z�b�g
	UIManager::GetInstance().Reset();
	//�G�t�F�N�g�����Z�b�g
	EffekseerManager::GetInstance().Reset();
	//�A�N�^�[�}�l�[�W���[�̍ăX�^�[�g
	m_actorManager->Restart();
	//�J�����̏�����
	m_camera->Init(index);
	//�t���O���Z�b�g
	m_isGameover = false;
	m_isGameClear = false;
	auto& saveDataManager = SaveDataManager::GetInstance();
	auto score = saveDataManager.GetScore();
	//�X�R�A�̏�����
	if(!score.expired())score.lock()->Init();
	//UI�쐬
	UIManager::GetInstance().CreateGameScoreUI(score);
	UIManager::GetInstance().CreateTimerUI(m_timer);
}
void GameManager::Continue()
{
	//�X�R�A�̏�����
	SaveDataManager::GetInstance().GetScore().lock()->Init();
	//�v���C���[�𕜊�
	m_actorManager->RevivalPlayer();
	//UI�̕`����~�߂�
	UIManager::GetInstance().AllStartDraw();
	//�t���O���Z�b�g
	m_isGameover = false;
	m_isGameClear = false;
	m_isResult = false;
}

void GameManager::InitLight()
{
	m_lightHandles.emplace_back(CreateDirLightHandle(kLightDir));
	//m_lightHandles.emplace_back(CreateDirLightHandle(kLightDir2));
}

void GameManager::InitShadow()
{
	//�V���h�E�}�b�v�n���h���̍쐬
	m_shadowMapHandle = MakeShadowMap(kShadowMapWidth, kShadowMapHeight);
}

void GameManager::UpdateShadowDrawArea()
{
	auto shadowMinPos = m_camera->GetPos();
	shadowMinPos.x += -kShadowMapHorizon;
	shadowMinPos.y = kShadowMapVerticalMin;
	shadowMinPos.z += -kShadowMapHorizon;
	auto shadowMaxPos = m_camera->GetPos();
	shadowMaxPos.x += kShadowMapHorizon;
	shadowMaxPos.y = kShadowMapVerticalMax;
	shadowMaxPos.z += kShadowMapHorizon;
	SetShadowMapDrawArea(m_shadowMapHandle, shadowMinPos.ToDxLibVector(), shadowMaxPos.ToDxLibVector());
}

void GameManager::UpdateGameStart()
{
	if (!m_actorManager->GetPlayer().expired())
	{
		//�v���C���[���X�^�[�g��Ԃ̎�
		if (m_actorManager->GetPlayer().lock()->IsStartAnim())
		{
			//�^�C�}�[���~�߂�
			m_timer->StopUpdate();
		}
		//�v���C���[���X�^�[�g��Ԃ��I��������
		else
		{
			//�^�C�}�[���J�n
			m_timer->StartUpdate();
		}
	}
}