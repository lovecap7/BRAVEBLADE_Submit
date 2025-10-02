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
	//非同期開始
	LoadingManager::GetInstance().StartLoading();
	m_gameManager = std::make_unique<GameManager>();
	//ロード初期化
	LoadingManager::GetInstance().Init();
	//非同期終了
	LoadingManager::GetInstance().StopLoading();
}

StageScene::~StageScene()
{
}

void StageScene::Init()
{
	m_gameManager->Init(m_stageIndex);
	auto& fader = Fader::GetInstance();
	//だんだん明るく
	fader.FadeIn(kFadeSpeed);
	//ステージによってBGMを変更
	InitBGM();
}

void StageScene::Update()
{
	auto& input = Input::GetInstance();
#if _DEBUG
	//デバッグシーン
	if (input.IsTrigger("SceneChange"))
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<DebugScene>(m_controller));
		return;
	}
	//ゲームクリア
	if (input.IsTrigger("Clear"))
	{
		m_controller.PushScene(std::make_shared<GameClearScene>(m_controller, m_stageIndex));
		return;
	}
#endif
	//ゲームの更新
	m_gameManager->Update();
	//ゲームクリアしたときの処理
	if (m_gameManager->IsResult() || (input.IsTrigger("Pause") && m_gameManager->IsGameClear()))
	{
		m_controller.PushScene(std::make_shared<GameClearScene>(m_controller, m_stageIndex));
		return;
	}
	//ゲームオーバーしたときの処理
	else if (m_gameManager->IsGameover())
	{
		m_controller.PushScene(std::make_shared<GameoverScene>(m_controller, m_stageIndex));
		return;
	}
	if (input.IsTrigger("Pause"))
	{
		//ポーズ
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
	//ステージによってBGMを変更
	InitBGM();
	auto& fader = Fader::GetInstance();
	//だんだん明るく
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