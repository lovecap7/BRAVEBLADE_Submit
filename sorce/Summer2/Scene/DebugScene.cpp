#include "DebugScene.h"
#include "TitleScene.h"
#include "SelectStageScene.h"
#include "StageScene.h"
#include <DxLib.h>
#include  "../General/Input.h"
#include  "../General/Fader.h"
#include "SceneController.h"
#include <memory>

namespace
{
	constexpr int kSceneNum = 7;
}

DebugScene::DebugScene(SceneController& controller):
	SceneBase(controller),
	m_selectSceneIndex(0)
{
}

DebugScene::~DebugScene()
{
}

void DebugScene::Init()
{
	auto& fader = Fader::GetInstance();
	//だんだん明るく
	fader.FadeIn();
}

void DebugScene::Update()
{
	auto& input = Input::GetInstance();
#if _DEBUG
	if (input.IsTrigger("Up")) --m_selectSceneIndex;
	if (input.IsTrigger("Down")) ++m_selectSceneIndex;

	if (m_selectSceneIndex < 0)
	{
		m_selectSceneIndex = kSceneNum - 1;
	}
	else if (m_selectSceneIndex >= kSceneNum)
	{
		m_selectSceneIndex = 0;
	}
	auto& fader = Fader::GetInstance();
	if (input.IsTrigger("Ok"))
	{
		//だんだん暗く
		fader.FadeOut();
	}
	if (fader.IsFinishFadeOut())
	{
		switch (m_selectSceneIndex)
		{
		case 0:
			//次のシーンへ
			m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
			return;
			break;
		case 1:
			//次のシーンへ
			m_controller.ChangeScene(std::make_shared<SelectStageScene>(m_controller, Stage::StageIndex::Stage1));
			return;
			break;
		case 2:
			//次のシーンへ
			m_controller.ChangeScene(std::make_shared<StageScene>(m_controller,Stage::StageIndex::Stage1));
			return;
			break;
		case 3:
			//次のシーンへ
			m_controller.ChangeScene(std::make_shared<StageScene>(m_controller, Stage::StageIndex::Stage2));
			return;
			break;
		case 4:
			//次のシーンへ
			m_controller.ChangeScene(std::make_shared<StageScene>(m_controller, Stage::StageIndex::Stage3));
			return;
			break;
		default:
			break;
		}
	}
	
#endif
}

void DebugScene::Draw()
{
	DrawString(0, 0, L"Debug Scene", 0xff0000);
	DrawString(100, 50, L"Title Scene", 0xffffff);
	DrawString(100, 70, L"SelectStage Scene", 0xffffff);
	DrawString(100, 90, L"Stage1 Scene", 0xffffff);
	DrawString(100, 110, L"Staeg2 Scene", 0xffffff);
	DrawString(100, 130, L"Stage3 Scene", 0xffffff);
	switch (m_selectSceneIndex)
	{
	case 0:
		DrawString(100, 50, L"Title Scene", 0xff0000);
		break;
	case 1:
		DrawString(100, 70, L"SelectStage Scene", 0xff0000);
		break;
	case 2:
		DrawString(100, 90, L"Stage1 Scene", 0xff0000);
		break;
	case 3:
		DrawString(100, 110, L"Staeg2 Scene", 0xff0000);
		break;
	case 4:
		DrawString(100, 130, L"Stage3 Scene", 0xff0000);
		break;
	default:
		break;
	}
}

void DebugScene::End()
{
}

void DebugScene::Restart()
{
}
