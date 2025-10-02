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
	constexpr int kFrameMargin = 10;//ゲーム画面からポーズ画面までの幅
	//各メニューの位置
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
	//描画停止
	UIManager::GetInstance().AllStopDraw();
	//エフェクト停止
	EffekseerManager::GetInstance().StopEffect();
	//Physicsを止める
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
	//Physicsを開始
	Physics::GetInstance().StartUpdate();
	//エフェクト開始
	EffekseerManager::GetInstance().StartEffect();
	//描画停止
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
		//UI準備
		InitUI();
		m_update = &PauseScene::NormalUpdate;
		m_draw = &PauseScene::NormalDraw;
		return;
	}
}

void PauseScene::NormalUpdate()
{
	auto& input = Input::GetInstance();
	//Pボタンでポーズ解除
	if (input.IsTrigger("Pause") || input.IsTrigger("B"))
	{
		InitDisappear();
		m_menuSelectIndex = MenuIndex::RetrunGame;
		return;
	}
	//選択
	MenuSelect(input);
	if (input.IsTrigger("A"))
	{
		//設定を選んだとき
		if (m_menuSelectIndex == MenuIndex::Option)
		{
			//オプション
			Option();
			//通常状態に戻す
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
			//ゲームに戻る
			RetrunGame();
			break;
		case MenuIndex::RestartGame:
			//ゲームを再スタート
			RestartGame();
			break;
		case MenuIndex::SelectStage:
			//セレクトシーン
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
	//背景を白っぽくする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(10, 10,//左上
		wSize - 10, hSize - 10,//右下
		0xffffff,//カラー
		true);//塗り潰す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ポーズ画面枠
	DrawBoxAA(10.0f, 10.0f,//左上
		wSize - 10.0f, hSize - 10.0f,//右下
		0xffffff,//カラー
		false,//塗り潰さない
		3.0f);//ちょっと太目の線
}

void PauseScene::ShiftingDraw()
{
	float wSize = Game::kScreenWidth;
	float hSize = Game::kScreenHeight;

	//元の高さを計算する
	const int frameHeight = hSize - 10 - 10;
	//真ん中のY座標を計算する
	const int middleY = (10 + hSize - 10) / 2;//真ん中のY座標

	//割合計算
	float rate = static_cast<float>(m_countFrame) / static_cast<float>(kAppearInterval);

	//現在の時間であり得るべき高さの半分を計算します
	int halfHeight = static_cast<int>((frameHeight * rate) * 0.5f);

	//ここから、top(上)とbottom(下)を計算します
	int top = middleY - halfHeight;
	int bottom = middleY + halfHeight;
	//背景を白っぽくする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
	DrawBox(10, top,//左上
		wSize - 10, bottom,//右下
		0xffffff,//カラー
		true);//塗り潰す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ポーズ画面枠
	DrawBoxAA(10.0f, static_cast<float>(top),//左上
		wSize - 10.0f, static_cast<float>(bottom),//右下
		0xffffff,//カラー
		false,//塗り潰さない
		3.0f);//ちょっと太目の線
}
void PauseScene::RetrunGame()
{
	m_controller.PopScene();
	return;
}

void PauseScene::RestartGame()
{
	//下のシーンをリスタートする
	m_controller.RestartBaseScene();
	//自分を消す
	m_controller.PopScene();
	return;
}

void PauseScene::Option()
{
	//オプション
	m_controller.PushScene(std::make_shared<OptionScene>(m_controller));
	return;
}

void PauseScene::SelectStage()
{
	//セレクトシーンへ
	m_controller.ChangeBaseScene(std::make_shared<SelectStageScene>(m_controller,m_stageIndex));
	//自分を消す
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
	//ゲームに戻る
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
	//登録
	for (auto& menuUI : m_menuUIs)
	{
		menuUI.second.lock()->Init();
	}
}
void PauseScene::MenuSelect(Input& input)
{
	int menuIndex = static_cast<int>(m_menuSelectIndex);
	//選ぶ
	if (input.IsRepeate("Up"))--menuIndex;
	if (input.IsRepeate("Down"))++menuIndex;
	menuIndex = MathSub::ClampInt(menuIndex, static_cast<int>(MenuIndex::RetrunGame), static_cast<int>(MenuIndex::SelectStage));
	m_menuSelectIndex = static_cast<MenuIndex>(menuIndex);
	//選んだものを拡大
	for (auto& menuUI : m_menuUIs)
	{
		menuUI.second.lock()->SetIsSelect(false);
	}
	m_menuUIs[m_menuSelectIndex].lock()->SetIsSelect(true);
}
