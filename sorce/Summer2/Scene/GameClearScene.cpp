#include "GameClearScene.h"
#include "SelectStageScene.h"
#include "../General/Input.h"
#include "SceneController.h"
#include<DxLib.h>
#include "../General/game.h"
#include "../General/Collision/Physics.h"
#include "../General/Sound/SoundManager.h"
#include "../Game/GameRule/Score.h"
#include "../General/Fader.h"
#include "../General/Sound/SE.h"
#include "../General/Effect/EffekseerManager.h"
#include "../Game/UI/UIManager.h"
#include "../Game/UI/Result/ResultUI.h"
#include "../Game/UI/Result/ResultScoreUI.h"
#include "../Game/UI/Result/ResultRankingScore.h"
#include "../SaveData/SaveDataManager.h"

namespace {
	constexpr int kAppearInterval = 60;
	constexpr int kAlphaRate = 2;
	constexpr int kFrameMargin = 10;//ゲーム画面からポーズ画面までの幅
}

GameClearScene::GameClearScene(SceneController& controller, Stage::StageIndex index):
	SceneBase(controller),
	m_stageIndex(index),
	m_update(&GameClearScene::AppearUpdate),
	m_draw(&GameClearScene::NormalDraw),
	m_countFrame(0)
{
	//勝利BGM
	SoundManager::GetInstance().PlayBGM("ResultBGM");
	//加算SE
	m_addScoreSE = SoundManager::GetInstance().PlayLoopSE("AddScore");
	//止める
	m_addScoreSE.lock()->Stop();
}

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Init()
{
	auto& uiManager = UIManager::GetInstance();
	//UI表示
	uiManager.StartDraw();
	uiManager.StartUpdate();
	//一度UIをリセット
	uiManager.Reset();
	//Physicsを止める
	Physics::GetInstance().StopUpdate();
	//エフェクト停止
	EffekseerManager::GetInstance().StopEffect();
	//クリアしたことを記録する
	auto& saveDataManager = SaveDataManager::GetInstance();
	saveDataManager.SaveClearStage(m_stageIndex);
	//UI
	std::make_shared<ResultUI>()->Init();
}

void GameClearScene::Update()
{
	(this->*m_update)();
}

void GameClearScene::Draw()
{
	(this->*m_draw)();
}
void GameClearScene::End()
{
	//Physicsを開始
	Physics::GetInstance().StartUpdate();
	//エフェクト開始
	EffekseerManager::GetInstance().StartEffect();
	//SE削除
	if(!m_addScoreSE.expired())m_addScoreSE.lock()->Delete();
}

void GameClearScene::Restart()
{
}

void GameClearScene::AppearUpdate()
{
	++m_countFrame;
	if (m_countFrame > kAppearInterval)
	{
		//止める
		m_addScoreSE.lock()->Play();
		//リザルトUI
		InitResult1UI();
		m_countFrame = kAppearInterval;
		m_update = &GameClearScene::Result1Update;
		return;
	}
}

void GameClearScene::Result1Update()
{
	auto& input = Input::GetInstance();
	bool isAllMax = true;
	for (auto& scoreUI : m_scoreUIList)
	{
		if (!scoreUI.lock()->IsViewScoreMax())
		{
			isAllMax = false;
			break;
		}
	}
	//全てのスコアUIが最大値になったら
	if (isAllMax)
	{
		//止める
		m_addScoreSE.lock()->Stop();
		//Aボタンで次へ
		if (input.IsTrigger("A"))
		{
			//止める
			m_addScoreSE.lock()->Play();
			//ここまで来たら次の状態へ
			//ランキングUI
			InitResult2UI();
			//次の状態
			m_update = &GameClearScene::Result2Update;
			return;
		}
	}
}

void GameClearScene::Result2Update()
{
	auto& input = Input::GetInstance();
	bool isAllMax = true;
	for (auto& scoreUI : m_scoreUIList)
	{
		if (!scoreUI.lock()->IsViewScoreMax())
		{
			isAllMax = false;
			break;
		}
	}
	//全てのスコアUIが最大値になったら
	if (isAllMax)
	{
		//止める
		m_addScoreSE.lock()->Stop();
		//Aボタンで次へ
		if (input.IsTrigger("A"))
		{
			auto& fader = Fader::GetInstance();
			//だんだん暗く
			fader.FadeOut();
			//次の状態
			m_update = &GameClearScene::DisappearUpdate;
			return;
		}
	}
}

void GameClearScene::DisappearUpdate()
{
	auto& fader = Fader::GetInstance();
	//暗くなったら
	if (fader.IsFinishFadeOut())
	{
		//自分の下になってるシーンを切り替える
		m_controller.ChangeBaseScene(std::make_shared<SelectStageScene>(m_controller,m_stageIndex));
		m_controller.PopScene();//自分は消える
		return;
	}
}

void GameClearScene::NormalDraw()
{
	//背景を白っぽくする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlphaRate * m_countFrame);
	DrawBox(0, 0,//左上
		Game::kScreenWidth, Game::kScreenHeight,//右下
		0xffffff,//カラー
		true);//塗り潰す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameClearScene::InitResult1UI()
{
	//ローダー
	auto loader = std::make_shared<CSVDataLoader>();
	m_scoreUiData = loader->LoadResultScoreUIDataCSV();
	//スコア
	auto score = SaveDataManager::GetInstance().GetScore().lock();

	//データの数だけUIを用意
	for (auto& data : m_scoreUiData)
	{
		std::shared_ptr<ResultScoreUI> scoreUI;
		float value = 0.0f;
		if (data.name == "TotalScore")
		{
			value = score->GetScore();
		}
		else if (data.name == "KillScore")
		{
			value = score->GetKillScore();
		}
		else if (data.name == "ItemScore")
		{
			value = score->GetItemScore();
		}
		else if (data.name == "TimeScore")
		{
			value = score->GetTimeScore();
		}
		else if (data.name == "HPScore")
		{
			value = score->GetHPScore();
		}
		//データにないものは飛ばす
		else
		{
			continue;
		}
		scoreUI = std::make_shared<ResultScoreUI>(value, data.pos, data.text);
		scoreUI->Init();
		m_scoreUIList.emplace_back(scoreUI);
	}
}

void GameClearScene::InitResult2UI()
{
	//今のUIを削除
	for (auto& scoreUI : m_scoreUIList)
	{
		if (auto ui = scoreUI.lock())
		{
			//削除
			ui->Delete();
		}
	}
	m_scoreUIList.clear();
	//スコア
	auto score = SaveDataManager::GetInstance().GetScore().lock();

	//データの数だけUIを用意
	for (auto& data : m_scoreUiData)
	{
		std::shared_ptr<ResultScoreUI> scoreUI;
		float value = 0.0f;
		if (data.name == "HighScore1")
		{
			value = score->GetHighScore(m_stageIndex)[0];
		}
		else if (data.name == "HighScore2")
		{
			value = score->GetHighScore(m_stageIndex)[1];
		}
		else if (data.name == "HighScore3")
		{
			value = score->GetHighScore(m_stageIndex)[2];
		}
		//データにないものは飛ばす
		else
		{
			continue;
		}
		scoreUI = std::make_shared<ResultRankingScore>(value, data.pos, data.text, score->GetScore());
		scoreUI->Init();
		m_scoreUIList.emplace_back(scoreUI);
	}
}
