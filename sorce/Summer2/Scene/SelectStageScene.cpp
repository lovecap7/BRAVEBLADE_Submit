#include "SelectStageScene.h"
#include "TitleScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include "../General/Fader.h"
#include "../General/CSVDataLoader.h"
#include "../General/StageFigure.h"
#include "../General/Sound/SoundManager.h"
#include "../Game/Actors/Character/Player/SelectStagePlayer.h"
#include "../Game/Camera/SelectStageCamera/SelectStageCamera.h"
#include "../Game/UI/Select/SelectStageBackUI.h"
#include "../Game/UI/Select/SelectStageRankingUI.h"
#include "../Game/UI/UIManager.h"
#include "../Game/UI/LeftArrowUI.h"
#include "../SaveData/SaveDataManager.h"
#include "StageScene.h"
#include <memory>
#include <DxLib.h>
#if _DEBUG
//デバッグモード
#include "DebugScene.h"
#endif

namespace
{
	constexpr float kFadeSpeed = 2.0f; // フェード速度
	//矢印UIの位置
	const Vector2 kLeftArrowPos = Vector2{ 130, 520 };
	const Vector2 kRightArrowPos = Vector2{ 1150, 520 };
}

SelectStageScene::SelectStageScene(SceneController& controller, Stage::StageIndex index):
	SceneBase(controller),
	m_stageIndex(static_cast<int>(index)),
	m_unlockStageIndex(Stage::StageIndex::Stage1),
	m_isDecide(false)
{
	//CSVデータローダー
	auto csvLodader = std::make_shared<CSVDataLoader>();
	//CSVから座標データを読み込む
	auto tData = csvLodader->LoadActorDataCSV("Data/CSV/SelectStageTransformData.csv");
	//ステージの位置
	for (auto& data : tData)
	{
		if (data.name == "1")
		{
			m_stagePos[Stage::StageIndex::Stage1] = data.pos;
		}
		else if (data.name == "2")
		{
			m_stagePos[Stage::StageIndex::Stage2] = data.pos;
		}
		else if (data.name == "3")
		{
			m_stagePos[Stage::StageIndex::Stage3] = data.pos;
		}
	}
	//カメラ
	m_camera = std::make_unique<SelectStageCamera>(m_stagePos[static_cast<Stage::StageIndex>(m_stageIndex)]);
	//プレイヤー
	m_player = std::make_unique<SelectStagePlayer>(m_camera->GetPos(),m_stagePos[static_cast<Stage::StageIndex>(m_stageIndex)]);
	//ステージのフィギュア
	for (int i = 0; i < m_stageFigures.size(); ++i)
	{
		//ステージのフィギュアを作成
		m_stageFigures[i] = std::make_shared<StageFigure>();
	}
}

SelectStageScene::~SelectStageScene()
{
}

void SelectStageScene::Init()
{
	//解放しているステージまでしか選べなくするための処理
	NowUnlockStage();

	auto& fader = Fader::GetInstance();
	//だんだん明るく
	fader.FadeIn(kFadeSpeed);
	//カメラの初期化
	m_camera->Init();
	//プレイヤーの初期化
	m_player->Init();
	//ステージのフィギュアの初期化
	m_stageFigures[0]->Init(MV1LoadModel(L"Data/Model/Stage/Select/Stage1Model.mv1"),m_stagePos[Stage::StageIndex::Stage1]);
	m_stageFigures[1]->Init(MV1LoadModel(L"Data/Model/Stage/Select/Stage2Model.mv1"),m_stagePos[Stage::StageIndex::Stage2]);
	m_stageFigures[2]->Init(MV1LoadModel(L"Data/Model/Stage/Select/Stage3Model.mv1"),m_stagePos[Stage::StageIndex::Stage3]);

	//背景(生成する順番は描画下から)
	auto back1 = std::make_shared<SelectStageBackUI>(LoadGraph(L"Data/UI/Back/Stage1Back.png"));
	auto back2 = std::make_shared<SelectStageBackUI>(LoadGraph(L"Data/UI/Back/Stage2Back.png"));
	auto back3 = std::make_shared<SelectStageBackUI>(LoadGraph(L"Data/UI/Back/Stage3Back.png"));
	back3->Init();
	back2->Init();
	back1->Init();
	m_stage1Back = back1;
	m_stage2Back = back2;
	//ランキング
	auto rankingUI = std::make_shared<SelectStageRankingUI>(static_cast<Stage::StageIndex>(m_stageIndex));
	rankingUI->Init();
	m_stageRankingUI = rankingUI;
	//矢印UI
	auto leftArrowUI = std::make_shared<LeftArrowUI>(kLeftArrowPos, false);
	auto rightArrowUI = std::make_shared<LeftArrowUI>(kRightArrowPos, true);
	leftArrowUI->Init();
	rightArrowUI->Init();
	leftArrowUI->SetIsSelect(true);
	rightArrowUI->SetIsSelect(true);
	m_leftArrowUI = leftArrowUI;
	m_rightArrowUI = rightArrowUI;
	//BGM
	SoundManager::GetInstance().PlayBGM("SelectStageBGM");
}

void SelectStageScene::Update()
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
	//ステージ全開放
	if (input.IsTrigger("AllClearStage"))
	{
		auto& saveDatas = SaveDataManager::GetInstance();
		saveDatas.AllClearStage();
		NowUnlockStage();
	}
#endif
	auto& fader = Fader::GetInstance();
	//真っ暗になったら
	if (fader.IsFinishFadeOut())
	{
		//決定してるなら
		if (m_isDecide)
		{
			//次のシーンへ
			m_controller.ChangeScene(std::make_shared<StageScene>(m_controller, static_cast<Stage::StageIndex>(m_stageIndex)));
			return;
		}
		else
		{
			//タイトルへ
			m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
			return;
		}
	}
	if (!fader.IsFadeNow())
	{
		//決定ボタンをおしたら
		if (input.IsTrigger("A"))
		{
			//決定SE
			SoundManager::GetInstance().PlayOnceSE("Decide");
			m_isDecide = true;
			//だんだん暗く
			fader.FadeOut(kFadeSpeed);
		}
		//戻るボタンをおしたら
		else if (input.IsTrigger("B"))
		{
			//決定SE
			SoundManager::GetInstance().PlayOnceSE("Cancel");
			//だんだん暗く
			fader.FadeOut(kFadeSpeed);
		}
	}
	//ステージを選ぶ
	SelectStageIndex(input);
	//ステージに合わせて背景を動かす
	ChangeBack();
	//カメラの更新
	m_camera->Update(m_stagePos[static_cast<Stage::StageIndex>(m_stageIndex)]);
	//プレイヤーの更新
	m_player->Update(m_camera->GetPos(), m_stagePos[static_cast<Stage::StageIndex>(m_stageIndex)]);
	//ステージのフィギュアの更新
	for (auto& stageFigure : m_stageFigures)
	{
		stageFigure->Update();
	}
}

void SelectStageScene::Draw()
{
#if _DEBUG
	DrawString(0, 0, L"SelectStage Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);

	DrawFormatString(0, 80, 0xffff00, L"StageIndex : %d", m_stageIndex);
	DrawFormatString(0, 160, 0xffff00, L"UnlockStageIndex : %d", static_cast<int>(m_unlockStageIndex));
#endif
	//プレイヤーの描画
	m_player->Draw();
	//ステージのフィギュアの描画
	for (auto& stageFigure : m_stageFigures)
	{
		stageFigure->Draw();
	}	
}

void SelectStageScene::End()
{
	//プレイヤーの終了
	m_player->End();
	//ステージのフィギュアの終了
	for (auto& stageFigure : m_stageFigures)
	{
		stageFigure->End();
	}
	//現状のデータをセーブ
	SaveDataManager::GetInstance().Save();
	//削除
	UIManager::GetInstance().Reset();

}

void SelectStageScene::Restart()
{
}

void SelectStageScene::SelectStageIndex(Input& input)
{
	auto lastIndex = m_stageIndex;
	//解放されてるステージまでしか選べない
	if (input.IsTrigger("Left"))--m_stageIndex;
	if (input.IsTrigger("Right"))++m_stageIndex;
	m_stageIndex = MathSub::ClampInt(m_stageIndex, static_cast<int>(Stage::StageIndex::Stage1), static_cast<int>(m_unlockStageIndex));
	if (m_stageIndex != lastIndex)
	{
		//セレクトSE
		SoundManager::GetInstance().PlayOnceSE("Select");
	}
	if (m_stageRankingUI.expired())return;
	m_stageRankingUI.lock()->SetStageIndex(static_cast<Stage::StageIndex>(m_stageIndex));

	if (m_leftArrowUI.expired() || m_rightArrowUI.expired())return;
	//一つもクリアしていないなら矢印は右矢印をロック
	if (static_cast<int>(m_unlockStageIndex) == static_cast<int>(Stage::StageIndex::Stage1))
	{
		m_leftArrowUI.lock()->SetIsDraw(false);
		m_rightArrowUI.lock()->SetIsDraw(true);
		m_rightArrowUI.lock()->SetIsLock(true);
		return;
	}
	//ステージの選択できる方向によって矢印の表示を変える
	if(m_stageIndex == static_cast<int>(Stage::StageIndex::Stage1))
	{
		//右矢印のみ表示
		m_leftArrowUI.lock()->SetIsDraw(false);
		m_leftArrowUI.lock()->SetIsLock(false);
		m_rightArrowUI.lock()->SetIsDraw(true);
		m_rightArrowUI.lock()->SetIsLock(false);
	}
	//ステージ3を選択しているなら
	else if (m_stageIndex == static_cast<int>(Stage::StageIndex::Stage3))
	{
		//左矢印のみ表示
		m_leftArrowUI.lock()->SetIsDraw(true);
		m_leftArrowUI.lock()->SetIsLock(false);
		m_rightArrowUI.lock()->SetIsDraw(false);
		m_rightArrowUI.lock()->SetIsLock(false);
	}
	//ステージ2を選択していて、ステージ3が解放されていないなら
	else if (m_stageIndex == static_cast<int>(m_unlockStageIndex))
	{
		//左表示　右ロック
		m_leftArrowUI.lock()->SetIsDraw(true);
		m_leftArrowUI.lock()->SetIsLock(false);
		m_rightArrowUI.lock()->SetIsDraw(true);
		m_rightArrowUI.lock()->SetIsLock(true);
	}
	//それ以外なら両方表示
	else
	{
		m_leftArrowUI.lock()->SetIsDraw(true);
		m_leftArrowUI.lock()->SetIsLock(false);
		m_rightArrowUI.lock()->SetIsDraw(true);
		m_rightArrowUI.lock()->SetIsLock(false);
	}
}

void SelectStageScene::ChangeBack()
{
	if (m_stageIndex == static_cast<int>(Stage::StageIndex::Stage1))
	{
		m_stage1Back.lock()->SetAppear(true);
		m_stage2Back.lock()->SetAppear(true);
	}
	else if (m_stageIndex == static_cast<int>(Stage::StageIndex::Stage2))
	{
		m_stage1Back.lock()->SetAppear(false);
		m_stage2Back.lock()->SetAppear(true);
	}
	else
	{
		m_stage1Back.lock()->SetAppear(false);
		m_stage2Back.lock()->SetAppear(false);
	}
}

void SelectStageScene::NowUnlockStage()
{
	auto& saveDatas = SaveDataManager::GetInstance();
	//ステージ1をクリアしていないなら
	if (!saveDatas.IsClearStage(Stage::StageIndex::Stage1))
	{
		//ステージ1しか選べなくする
		m_unlockStageIndex = Stage::StageIndex::Stage1;
	}
	//ステージ2をクリアしていないなら
	else if (!saveDatas.IsClearStage(Stage::StageIndex::Stage2))
	{
		//ステージ2までしか選べなくする
		m_unlockStageIndex = Stage::StageIndex::Stage2;
	}
	//ここまで来たら全て解放
	else
	{
		//全ステージ解放
		m_unlockStageIndex = Stage::StageIndex::Stage3;
	}
}