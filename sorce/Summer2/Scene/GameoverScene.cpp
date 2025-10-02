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
	constexpr int kFrameMargin = 10;//ゲーム画面からポーズ画面までの幅
	//各メニューの位置
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
	//Physicsを止める
	Physics::GetInstance().StopUpdate();
	//エフェクト停止
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
	//Physicsを開始
	Physics::GetInstance().StartUpdate();
	//エフェクト開始
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
		//UIの準備
		InitUI();
		m_countFrame = kAppearInterval;
		m_update = &GameoverScene::NormalUpdate;
		return;
	}
}
void GameoverScene::NormalUpdate()
{
	auto& input = Input::GetInstance();
	//Aボタンで次へ
	if (input.IsTrigger("A")) 
	{
		//UI削除
		for (auto& menuUI : m_menuUIs)
		{
			menuUI.second.lock()->Delete();
		}
		m_gameoverUI.lock()->Delete();
		//続きからの場合
		if(m_menuSelectIndex == MenuIndex::Continue)
		{
			//続きから
			Continue();
			return;
		}
		//だんだん暗く
		auto& fader = Fader::GetInstance();
		fader.FadeOut();
		m_update = &GameoverScene::DisappearUpdate;
		return;
	}
	//メニューセレクト
	MenuSelect(input);
}

void GameoverScene::DisappearUpdate()
{
	auto& fader = Fader::GetInstance();
	//暗くなったら
	if (fader.IsFinishFadeOut())
	{
		switch (m_menuSelectIndex)
		{
		case MenuIndex::Restart:
			//最初から
			RestartBase();
			break;
		case MenuIndex::SelectStage:
			//ステージセレクト
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
	//背景を赤っぽくする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 10 * m_countFrame);
	DrawBox(0, 0,//左上
		Game::kScreenWidth, Game::kScreenHeight,//右下
		0xff5555,//カラー
		true);//塗り潰す
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
	//登録
	for (auto& menuUI : m_menuUIs)
	{
		menuUI.second.lock()->Init();
	}
}

void GameoverScene::MenuSelect(Input& input)
{
	int menuIndex = static_cast<int>(m_menuSelectIndex);
	//選ぶ
	if (input.IsRepeate("Up"))--menuIndex;
	if (input.IsRepeate("Down"))++menuIndex;
	menuIndex = MathSub::ClampInt(menuIndex, static_cast<int>(MenuIndex::Continue), static_cast<int>(MenuIndex::SelectStage));
	m_menuSelectIndex = static_cast<MenuIndex>(menuIndex);
	//選んだものを拡大
	for (auto& menuUI : m_menuUIs)
	{
		menuUI.second.lock()->SetIsSelect(false);
	}
	m_menuUIs[m_menuSelectIndex].lock()->SetIsSelect(true);
}

void GameoverScene::Continue()
{
	//自分の下になってるシーンを取得
	auto baseScene = m_controller.GetBaseScene();
	if (baseScene.expired())return;
	auto stageScene = std::dynamic_pointer_cast<StageScene>(baseScene.lock());
	//プレイヤーを復活
	stageScene->Continue();
	m_controller.PopScene();//自分は消える
	return;
}

void GameoverScene::RestartBase()
{
	//自分の下になってるシーンを初期化
	m_controller.RestartBaseScene();
	m_controller.PopScene();//自分は消える
	return;
}

void GameoverScene::SelectStage()
{
	//セレクトシーンへ
	m_controller.ChangeBaseScene(std::make_shared<SelectStageScene>(m_controller, m_stageIndex));
	//自分を消す
	m_controller.PopScene();
	return;
}
