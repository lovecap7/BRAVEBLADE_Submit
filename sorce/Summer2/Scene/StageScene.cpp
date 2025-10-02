#include "StageScene.h"
#include "../Game/GameManager.h"
#include "DebugScene.h"
#include "PauseScene.h"
#include "GameoverScene.h"
#include "GameClearScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include <DxLib.h>
#include <vector>
#include "../General/game.h"
#include "../General/Fader.h"
#include "../General/Sound/SoundManager.h"
#include "../General/LoadingManager.h"

namespace
{
	constexpr float kFadeSpeed = 20.0f;
}

StageScene::StageScene(SceneController& controller, Stage::StageIndex index):
	SceneBase(controller),
	m_stageIndex(index)
{
	//�񓯊��J�n
	LoadingManager::GetInstance().StartLoading();
	m_gameManager = std::make_unique<GameManager>();
	//���[�h������
	LoadingManager::GetInstance().Init();
	//�񓯊��I��
	LoadingManager::GetInstance().StopLoading();
}

StageScene::~StageScene()
{
}

void StageScene::Init()
{
	m_gameManager->Init(m_stageIndex);
	auto& fader = Fader::GetInstance();
	//���񂾂񖾂邭
	fader.FadeIn(kFadeSpeed);
	//�X�e�[�W�ɂ����BGM��ύX
	InitBGM();
}

void StageScene::Update()
{
	auto& input = Input::GetInstance();
#if _DEBUG
	//�f�o�b�O�V�[��
	if (input.IsTrigger("SceneChange"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<DebugScene>(m_controller));
		return;
	}
	//�Q�[���N���A
	if (input.IsTrigger("Clear"))
	{
		m_controller.PushScene(std::make_shared<GameClearScene>(m_controller, m_stageIndex));
		return;
	}
#endif
	//�Q�[���̍X�V
	m_gameManager->Update();
	//�Q�[���N���A�����Ƃ��̏���
	if (m_gameManager->IsResult() || (input.IsTrigger("Pause") && m_gameManager->IsGameClear()))
	{
		m_controller.PushScene(std::make_shared<GameClearScene>(m_controller, m_stageIndex));
		return;
	}
	//�Q�[���I�[�o�[�����Ƃ��̏���
	else if (m_gameManager->IsGameover())
	{
		m_controller.PushScene(std::make_shared<GameoverScene>(m_controller, m_stageIndex));
		return;
	}
	if (input.IsTrigger("Pause"))
	{
		//�|�[�Y
		m_controller.PushScene(std::make_shared<PauseScene>(m_controller, m_stageIndex));
		return;
	}
}

void StageScene::Draw()
{
	m_gameManager->Draw();
}

void StageScene::End()
{
	m_gameManager->End();
}

void StageScene::Restart()
{
	m_gameManager->Restart(m_stageIndex);
	//�X�e�[�W�ɂ����BGM��ύX
	InitBGM();
	auto& fader = Fader::GetInstance();
	//���񂾂񖾂邭
	fader.FadeIn(kFadeSpeed);
}
void StageScene::Continue()
{
	m_gameManager->Continue();
}

void StageScene::InitBGM()
{
	switch (m_stageIndex)
	{
	case Stage::StageIndex::Stage1:
		SoundManager::GetInstance().PlayBGM("Stage1BGM");
		break;
	case Stage::StageIndex::Stage2:
		SoundManager::GetInstance().PlayBGM("Stage2BGM");
		break;
	}
}