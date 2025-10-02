#include "PauseScene.h"
#include "SelectStageScene.h"
#include "OptionScene.h"
#include "../General/Input.h"
#include "SceneController.h"
#include<DxLib.h>
#include "../General/game.h"
#include "../General/Collision/Physics.h"
#include "../Game/UI/UIManager.h"
#include "../Game/UI/MenuUI.h"
#include "../General/Effect/EffekseerManager.h"

namespace {
	constexpr int kAppearInterval = 20;
	constexpr int kFrameMargin = 10;//�Q�[����ʂ���|�[�Y��ʂ܂ł̕�
	//�e���j���[�̈ʒu
	constexpr float kPauseY = 100;
	constexpr float kRetrunGameY = 250;
	constexpr float kRestartGameY = 370;
	constexpr float kOptionGameY = 490;
	constexpr float kSelectStageY = 610;
}


PauseScene::PauseScene(SceneController& controller, Stage::StageIndex index):
	SceneBase(controller),
	m_update(&PauseScene::AppearUpdate),
	m_draw(&PauseScene::ShiftingDraw),
	m_countFrame(0),
	m_menuSelectIndex(MenuIndex::RetrunGame),
	m_stageIndex(index)
{
}

PauseScene::~PauseScene()
{
}

void PauseScene::Init()
{
	//�`���~
	UIManager::GetInstance().AllStopDraw();
	//�G�t�F�N�g��~
	EffekseerManager::GetInstance().StopEffect();
	//Physics���~�߂�
	Physics::GetInstance().StopUpdate();
}

void PauseScene::Update()
{
	(this->*m_update)();
}

void PauseScene::Draw()
{
	(this->*m_draw)();
}

void PauseScene::End()
{
	//Physics���J�n
	Physics::GetInstance().StartUpdate();
	//�G�t�F�N�g�J�n
	EffekseerManager::GetInstance().StartEffect();
	//�`���~
	UIManager::GetInstance().AllStartDraw();
}

void PauseScene::Restart()
{
}

void PauseScene::AppearUpdate()
{
	++m_countFrame;
	if (m_countFrame > kAppearInterval) 
	{
		//UI����
		InitUI();
		m_update = &PauseScene::NormalUpdate;
		m_draw = &PauseScene::NormalDraw;
		return;
	}
}

void PauseScene::NormalUpdate()
{
	auto& input = Input::GetInstance();
	//P�{�^���Ń|�[�Y����
	if (input.IsTrigger("Pause") || input.IsTrigger("B"))
	{
		InitDisappear();
		m_menuSelectIndex = MenuIndex::RetrunGame;
		return;
	}
	//�I��
	MenuSelect(input);
	if (input.IsTrigger("A"))
	{
		//�ݒ��I�񂾂Ƃ�
		if (m_menuSelectIndex == MenuIndex::Option)
		{
			//�I�v�V����
			Option();
			//�ʏ��Ԃɖ߂�
			m_update = &PauseScene::NormalUpdate;
			m_draw = &PauseScene::NormalDraw;
			return;
		}

		InitDisappear();
		return;
	}

}

void PauseScene::DisappearUpdate()
{
	--m_countFrame;
	if (m_countFrame < 0) 
	{
		switch (m_menuSelectIndex)
		{
		case MenuIndex::RetrunGame:
			//�Q�[���ɖ߂�
			RetrunGame();
			break;
		case MenuIndex::RestartGame:
			//�Q�[�����ăX�^�[�g
			RestartGame();
			break;
		case MenuIndex::SelectStage:
			//�Z���N�g�V�[��
			SelectStage();
			break;
		default:
			break;
		}
		return;
	}
}

void PauseScene::NormalDraw()
{
	float wSize = Game::kScreenWidth;
	float hSize = Game::kScreenHeight;
	//�w�i�𔒂��ۂ�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(10, 10,//����
		wSize - 10, hSize - 10,//�E��
		0xffffff,//�J���[
		true);//�h��ׂ�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�|�[�Y��ʘg
	DrawBoxAA(10.0f, 10.0f,//����
		wSize - 10.0f, hSize - 10.0f,//�E��
		0xffffff,//�J���[
		false,//�h��ׂ��Ȃ�
		3.0f);//������Ƒ��ڂ̐�
}

void PauseScene::ShiftingDraw()
{
	float wSize = Game::kScreenWidth;
	float hSize = Game::kScreenHeight;

	//���̍������v�Z����
	const int frameHeight = hSize - 10 - 10;
	//�^�񒆂�Y���W���v�Z����
	const int middleY = (10 + hSize - 10) / 2;//�^�񒆂�Y���W

	//�����v�Z
	float rate = static_cast<float>(m_countFrame) / static_cast<float>(kAppearInterval);

	//���݂̎��Ԃł��蓾��ׂ������̔������v�Z���܂�
	int halfHeight = static_cast<int>((frameHeight * rate) * 0.5f);

	//��������Atop(��)��bottom(��)���v�Z���܂�
	int top = middleY - halfHeight;
	int bottom = middleY + halfHeight;
	//�w�i�𔒂��ۂ�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
	DrawBox(10, top,//����
		wSize - 10, bottom,//�E��
		0xffffff,//�J���[
		true);//�h��ׂ�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�|�[�Y��ʘg
	DrawBoxAA(10.0f, static_cast<float>(top),//����
		wSize - 10.0f, static_cast<float>(bottom),//�E��
		0xffffff,//�J���[
		false,//�h��ׂ��Ȃ�
		3.0f);//������Ƒ��ڂ̐�
}
void PauseScene::RetrunGame()
{
	m_controller.PopScene();
	return;
}

void PauseScene::RestartGame()
{
	//���̃V�[�������X�^�[�g����
	m_controller.RestartBaseScene();
	//����������
	m_controller.PopScene();
	return;
}

void PauseScene::Option()
{
	//�I�v�V����
	m_controller.PushScene(std::make_shared<OptionScene>(m_controller));
	return;
}

void PauseScene::SelectStage()
{
	//�Z���N�g�V�[����
	m_controller.ChangeBaseScene(std::make_shared<SelectStageScene>(m_controller,m_stageIndex));
	//����������
	m_controller.PopScene();
	return;
}
void PauseScene::InitDisappear()
{
	for (auto& menuUI : m_menuUIs)
	{
		menuUI.second.lock()->Delete();
	}
	m_pauseUI.lock()->Delete();
	//�Q�[���ɖ߂�
	m_update = &PauseScene::DisappearUpdate;
	m_draw = &PauseScene::ShiftingDraw;
	m_countFrame = kAppearInterval;
}
void PauseScene::InitUI()
{
	auto& uiManager = UIManager::GetInstance();
	auto pause = std::make_shared<MenuUI>(Vector2{ Game::kScreenCenterX,kPauseY }, uiManager.GetImageHandle("Pause"));
	pause->Init();
	m_pauseUI = pause;
	auto returnGame = std::make_shared<MenuUI>(Vector2{ Game::kScreenCenterX,kRetrunGameY }, uiManager.GetImageHandle("ReturnGame"));
	auto restartGame = std::make_shared<MenuUI>(Vector2{ Game::kScreenCenterX,kRestartGameY }, uiManager.GetImageHandle("RestartGame"));
	auto option = std::make_shared<MenuUI>(Vector2{ Game::kScreenCenterX,kOptionGameY }, uiManager.GetImageHandle("Option"));
	auto selectStage = std::make_shared<MenuUI>(Vector2{ Game::kScreenCenterX,kSelectStageY }, uiManager.GetImageHandle("SelectStage"));
	m_menuUIs[MenuIndex::RetrunGame] = returnGame;
	m_menuUIs[MenuIndex::RestartGame] = restartGame;
	m_menuUIs[MenuIndex::Option] = option;
	m_menuUIs[MenuIndex::SelectStage] = selectStage;
	//�o�^
	for (auto& menuUI : m_menuUIs)
	{
		menuUI.second.lock()->Init();
	}
}
void PauseScene::MenuSelect(Input& input)
{
	int menuIndex = static_cast<int>(m_menuSelectIndex);
	//�I��
	if (input.IsRepeate("Up"))--menuIndex;
	if (input.IsRepeate("Down"))++menuIndex;
	menuIndex = MathSub::ClampInt(menuIndex, static_cast<int>(MenuIndex::RetrunGame), static_cast<int>(MenuIndex::SelectStage));
	m_menuSelectIndex = static_cast<MenuIndex>(menuIndex);
	//�I�񂾂��̂��g��
	for (auto& menuUI : m_menuUIs)
	{
		menuUI.second.lock()->SetIsSelect(false);
	}
	m_menuUIs[m_menuSelectIndex].lock()->SetIsSelect(true);
}
