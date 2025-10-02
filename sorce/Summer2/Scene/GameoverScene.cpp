#include "GameoverScene.h"
#include "SelectStageScene.h"
#include "StageScene.h"
#include "../General/Input.h"
#include "SceneController.h"
#include<DxLib.h>
#include "../General/game.h"
#include "../General/Collision/Physics.h"
#include "../General/Effect/EffekseerManager.h"
#include "../General/Fader.h"
#include "../Game/UI/UIManager.h"
#include "../Game/UI/MenuUI.h"

namespace {
	constexpr int kAppearInterval = 20;
	constexpr int kFrameMargin = 10;//�Q�[����ʂ���|�[�Y��ʂ܂ł̕�
	//�e���j���[�̈ʒu
	constexpr float kGameoverY = 100;
	constexpr float kContinueY = 300;
	constexpr float kRestartY = 420;
	constexpr float kSelectStageY = 540;
}

GameoverScene::GameoverScene(SceneController& controller, Stage::StageIndex index):
	SceneBase(controller),
	m_update(&GameoverScene::AppearUpdate),
	m_draw(&GameoverScene::NormalDraw),
	m_countFrame(0),
	m_menuSelectIndex(MenuIndex::Continue),
	m_stageIndex(index)
{
}

GameoverScene::~GameoverScene()
{
}

void GameoverScene::Init()
{
	//Physics���~�߂�
	Physics::GetInstance().StopUpdate();
	//�G�t�F�N�g��~
	EffekseerManager::GetInstance().StopEffect();
}

void GameoverScene::Update()
{
	(this->*m_update)();
}

void GameoverScene::Draw()
{
	(this->*m_draw)();
}
void GameoverScene::End()
{
	//Physics���J�n
	Physics::GetInstance().StartUpdate();
	//�G�t�F�N�g�J�n
	EffekseerManager::GetInstance().StartEffect();
}

void GameoverScene::Restart()
{
}

void GameoverScene::AppearUpdate()
{
	++m_countFrame;
	if (m_countFrame > kAppearInterval)
	{
		//UI�̏���
		InitUI();
		m_countFrame = kAppearInterval;
		m_update = &GameoverScene::NormalUpdate;
		return;
	}
}
void GameoverScene::NormalUpdate()
{
	auto& input = Input::GetInstance();
	//A�{�^���Ŏ���
	if (input.IsTrigger("A")) 
	{
		//UI�폜
		for (auto& menuUI : m_menuUIs)
		{
			menuUI.second.lock()->Delete();
		}
		m_gameoverUI.lock()->Delete();
		//��������̏ꍇ
		if(m_menuSelectIndex == MenuIndex::Continue)
		{
			//��������
			Continue();
			return;
		}
		//���񂾂�Â�
		auto& fader = Fader::GetInstance();
		fader.FadeOut();
		m_update = &GameoverScene::DisappearUpdate;
		return;
	}
	//���j���[�Z���N�g
	MenuSelect(input);
}

void GameoverScene::DisappearUpdate()
{
	auto& fader = Fader::GetInstance();
	//�Â��Ȃ�����
	if (fader.IsFinishFadeOut())
	{
		switch (m_menuSelectIndex)
		{
		case MenuIndex::Restart:
			//�ŏ�����
			RestartBase();
			break;
		case MenuIndex::SelectStage:
			//�X�e�[�W�Z���N�g
			SelectStage();
			break;
		default:
			break;
		}
		return;
	}
}

void GameoverScene::NormalDraw()
{
	//�w�i��Ԃ��ۂ�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 10 * m_countFrame);
	DrawBox(0, 0,//����
		Game::kScreenWidth, Game::kScreenHeight,//�E��
		0xff5555,//�J���[
		true);//�h��ׂ�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameoverScene::InitUI()
{
	auto& uiManager = UIManager::GetInstance();
	auto gameoverUI = std::make_shared<MenuUI>(Vector2{ Game::kScreenCenterX,kGameoverY }, uiManager.GetImageHandle("Gameover"));
	gameoverUI->Init();
	m_gameoverUI = gameoverUI;
	auto continueUI = std::make_shared<MenuUI>(Vector2{ Game::kScreenCenterX,kContinueY }, uiManager.GetImageHandle("Continue"));
	auto restartGameUI = std::make_shared<MenuUI>(Vector2{ Game::kScreenCenterX,kRestartY }, uiManager.GetImageHandle("RestartGame"));
	auto selectStageUI = std::make_shared<MenuUI>(Vector2{ Game::kScreenCenterX,kSelectStageY }, uiManager.GetImageHandle("SelectStage"));
	m_menuUIs[MenuIndex::Continue] = continueUI;
	m_menuUIs[MenuIndex::Restart] = restartGameUI;
	m_menuUIs[MenuIndex::SelectStage] = selectStageUI;
	//�o�^
	for (auto& menuUI : m_menuUIs)
	{
		menuUI.second.lock()->Init();
	}
}

void GameoverScene::MenuSelect(Input& input)
{
	int menuIndex = static_cast<int>(m_menuSelectIndex);
	//�I��
	if (input.IsRepeate("Up"))--menuIndex;
	if (input.IsRepeate("Down"))++menuIndex;
	menuIndex = MathSub::ClampInt(menuIndex, static_cast<int>(MenuIndex::Continue), static_cast<int>(MenuIndex::SelectStage));
	m_menuSelectIndex = static_cast<MenuIndex>(menuIndex);
	//�I�񂾂��̂��g��
	for (auto& menuUI : m_menuUIs)
	{
		menuUI.second.lock()->SetIsSelect(false);
	}
	m_menuUIs[m_menuSelectIndex].lock()->SetIsSelect(true);
}

void GameoverScene::Continue()
{
	//�����̉��ɂȂ��Ă�V�[�����擾
	auto baseScene = m_controller.GetBaseScene();
	if (baseScene.expired())return;
	auto stageScene = std::dynamic_pointer_cast<StageScene>(baseScene.lock());
	//�v���C���[�𕜊�
	stageScene->Continue();
	m_controller.PopScene();//�����͏�����
	return;
}

void GameoverScene::RestartBase()
{
	//�����̉��ɂȂ��Ă�V�[����������
	m_controller.RestartBaseScene();
	m_controller.PopScene();//�����͏�����
	return;
}

void GameoverScene::SelectStage()
{
	//�Z���N�g�V�[����
	m_controller.ChangeBaseScene(std::make_shared<SelectStageScene>(m_controller, m_stageIndex));
	//����������
	m_controller.PopScene();
	return;
}
