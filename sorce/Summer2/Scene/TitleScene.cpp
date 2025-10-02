#include "TitleScene.h"
#include "OptionScene.h"
#include <Dxlib.h>
#include  "../General/Input.h"
#include "SceneController.h"
#include "SelectStageScene.h"
#include "../General/Collision/Physics.h"
#include "../Game/UI/UIManager.h"
#include "../Game/Camera/TitleCamera/TitleCamera.h"
#include "../Game/Actors/Character/Player/TitlePlayer.h"
#include "../Game/Actors/Stage/StageObjectDraw.h"
#include "../Game/Actors/Stage/Sky.h"
#include "../General/CSVDataLoader.h"
#include "../General/Fader.h"
#include "../General/Effect/EffekseerManager.h"
#include "../Game/UI/Title/TitleUI.h"
#include "../Game/UI/MenuUI.h"
#include "../Game/UI/SaveDataUI.h"
#include "../Game/UI/DialogUI.h"
#include "../General/Sound/SoundManager.h"
#include "../General/StringUtil.h"
#include "../Main/Application.h"
#include "../SaveData/SaveDataManager.h"
#include <memory>
#include <cassert>
#if _DEBUG
//デバッグモード
#include "DebugScene.h"
#endif

namespace
{
	//シャドウマップの描画サイズ
	constexpr int kShadowMapWidth = 1024 * 2;
	constexpr int kShadowMapHeight = 1024 * 2;
	//ライトの向き
	const VECTOR kLightDir = { 0.5f, -0.5f, 0.8f };
	//シャドウマップの範囲
	constexpr float kShadowMapHorizon = 2000.0f;
	constexpr float kShadowMapVerticalMin = -1.0f;
	constexpr float kShadowMapVerticalMax = 1000.0f;
	//フェードアウト
	constexpr float kFadeOutSpeed = 2.0f;
}


TitleScene::TitleScene(SceneController& controller):
	SceneBase(controller),
	m_update(&TitleScene::UpdateTitle),
	m_isDecide(false),
	m_menuIndex(MenuIndex::Continue),
	m_shadowMapHandle(-1),
	m_lightHandle(-1)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	//UIをリセット
	UIManager::GetInstance().Reset();
	//カメラ
	m_camera = std::make_unique<TitleCamera>();
	//プレイヤー
	m_player = std::make_shared<TitlePlayer>();
	//ハンドルロード
	LoadHandle();
	//CSV
	auto csvLoader = std::make_shared<CSVDataLoader>();
	//配置データ
	LoadStage(csvLoader);
	//UI
	InitUIs(csvLoader);
	//タイトル画面の初期化
	InitTitle();
	//エフェクト
	EffekseerManager::GetInstance().CreateTrackActorEffect("FieldEff", m_player);
	//カメラの初期化
	m_camera->Init();
	//プレイヤー
	m_player->Init();
	//ライトの初期化
	InitLight();
	//影の初期化
	InitShadow();
	auto& fader = Fader::GetInstance();
	//だんだん明るく
	fader.FadeIn();
	//BGM再生
	SoundManager::GetInstance().PlayBGM("TitleBGM");
}

void TitleScene::Update()
{
	auto& input = Input::GetInstance();
	if (m_shadowMapHandle >= 0) {
		//シャドウマップが想定するライトの方向もセット
		SetShadowMapLightDirection(m_shadowMapHandle, kLightDir);
	}
	//状態更新
	(this->*m_update)(input);
}

void TitleScene::Draw()
{
#if _DEBUG
	DrawString(0, 0, L"Title Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);
#endif
	//シャドウマップへの描画の準備
	ShadowMap_DrawSetup(m_shadowMapHandle);
	//プレイヤーの描画
	m_player->Draw();
	//ステージ描画
	for (auto& obj : m_stageObjects)
	{
		obj->Draw();
	}
	//シャドウマップへの描画を終了
	ShadowMap_DrawEnd();
	//描画に使用するシャドウマップを設定
	SetUseShadowMap(0, m_shadowMapHandle);
	//プレイヤーの描画
	m_player->Draw();
	//ステージ描画
	for (auto& obj : m_stageObjects)
	{
		obj->Draw();
	}
	//描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);
}

void TitleScene::End()
{
	//プレイヤーの終了
	m_player->End();
	//ハンドル削除
	AllDeleteHandle();
	//オブジェ削除
	AllDeleteStage();
	//ライトの削除
	DeleteLightHandle(m_lightHandle);
	//シャドウマップの削除
	DeleteShadowMap(m_shadowMapHandle);
	//UI
	UIManager::GetInstance().Reset();
	//エフェクト
	EffekseerManager::GetInstance().Reset();
}

void TitleScene::LoadHandle()
{
	m_stageHandles["Block_Grass"] = { MV1LoadModel(L"Data/Model/Stage/1/Block_Grass.mv1") };
	m_stageHandles["Grass1"] = { MV1LoadModel(L"Data/Model/Stage/Title/Grass1.mv1") };
	m_stageHandles["Rock1"] = { MV1LoadModel(L"Data/Model/Stage/Title/Rock1.mv1") };
	m_stageHandles["Rock2"] = { MV1LoadModel(L"Data/Model/Stage/Title/Rock2.mv1") };
	m_stageHandles["Tree1"] = { MV1LoadModel(L"Data/Model/Stage/Title/Tree1.mv1") };
	m_stageHandles["Flower1"] = { MV1LoadModel(L"Data/Model/Stage/Title/Flower1.mv1") };
	m_stageHandles["Sky"] = { MV1LoadModel(L"Data/Model/Stage/Sky/Sky_Daylight02.pmx") };
	//ロードに成功したかチェック
	for (auto& [key, value] : m_stageHandles) {
		assert(value >= 0);
	}
}
void TitleScene::LoadStage(std::shared_ptr<CSVDataLoader> csvLoader)
{
	//空を作成
	m_stageObjects.emplace_back(std::make_shared<Sky>(MV1DuplicateModel(m_stageHandles["Sky"])));
	auto datas = csvLoader->LoadActorDataCSV("Data/CSV/TitleTransformData.csv");
	//名前からオブジェクトを配置していく
	for (auto& data : datas)
	{
		if (data.name == "Block_Grass")
		{
			//大きさを1/100しないと大きすぎるので
			data.scale = VScale(data.scale, 0.01f);
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Block_Grass")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Grass1")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Grass1")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Rock1")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Rock1")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Rock2")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Rock2")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Tree1")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Tree1")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Flower1")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Flower1")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
	}
}

void TitleScene::AllDeleteHandle()
{
	for (auto& [key, value] : m_stageHandles) {
		if (value >= 0)
		{
			auto result = MV1DeleteModel(value);
			assert(result == 0);
		}
	}
	m_stageHandles.clear();
}

void TitleScene::AllDeleteStage()
{
	//ステージ描画
	for (auto& obj : m_stageObjects)
	{
		obj->End();
	}
	m_stageObjects.clear();
	m_player.reset();
	m_camera.reset();
}

void TitleScene::UpdateTitle(Input& input)
{
#if _DEBUG
	//デバッグシーン
	if (input.IsTrigger("SceneChange"))
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<DebugScene>(m_controller));
		return;
	}
#endif
	//何かボタンをおしたら
	if (input.IsTriggerAny())
	{
		//出現しきってるなら
		if (m_titleUI.lock()->IsAppered())
		{
			//決定SE
			SoundManager::GetInstance().PlayOnceSE("Decide");
			InitSelectMenu();
			return;
		}
		else
		{
			//完全に出現させる
			m_titleUI.lock()->DissolveRateMin();
		}
	}
	//共通の更新処理
	UpdateCommon();
	
}
void TitleScene::UpdateSelectMenu(Input& input)
{
	auto& fader = Fader::GetInstance();
	//戻るボタンをおしたら
	if (input.IsTrigger("B") && m_titleUI.lock()->IsAppered() && !fader.IsFadeNow())
	{
		//キャンセルSE
		SoundManager::GetInstance().PlayOnceSE("Cancel");
		//初期化処理
		InitTitle();
		return;
	}
	//共通の更新処理
	UpdateCommon();
	//上下にステックを動かしてメニューの項目を選択する
	SelectMenu(input);

}

void TitleScene::UpdateDialog(Input& input)
{
	auto& fader = Fader::GetInstance();
	//真っ暗になったら
	if (fader.IsFinishFadeOut())
	{
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<SelectStageScene>(m_controller,Stage::StageIndex::Stage1));
		return;
	}
	if (!fader.IsFadeNow())
	{
		if (input.IsTrigger("Left"))m_dialogUI.lock()->SelectYes();
		if (input.IsTrigger("Right"))m_dialogUI.lock()->SelectNo();
		//決定した時インデックスから処理を分岐
		if (input.IsTrigger("A"))
		{
			//"はい"を選んだ場合
			if (m_dialogUI.lock()->IsYes())
			{
				//決定SE
				SoundManager::GetInstance().PlayOnceSE("Decide");
				switch (m_menuIndex)
				{
				case MenuIndex::Continue:
					Continue();
					break;
				case MenuIndex::NewGame:
					NewGame();
					break;
				case MenuIndex::FinishGame:
					FinishGame();
					break;
				}
				return;
			}
			//"いいえ"を選んだ場合
			else
			{
				InitSelectMenu();
				//キャンセルSE
				SoundManager::GetInstance().PlayOnceSE("Cancel");
				m_update = &TitleScene::UpdateSelectMenu;
				return;
			}
		}
		if (input.IsTrigger("B"))
		{
			InitSelectMenu();
			//キャンセルSE
			SoundManager::GetInstance().PlayOnceSE("Cancel");
			m_update = &TitleScene::UpdateSelectMenu;
			return;
		}
	}
	//共通の更新処理
	UpdateCommon();
}

void TitleScene::SelectMenu(Input& input)
{
	//選択
	auto menuIndex = static_cast<int>(m_menuIndex);
	if (input.IsRepeate("Up"))	--menuIndex;
	if (input.IsRepeate("Down"))++menuIndex;
	//値が変化したら
	if (menuIndex != static_cast<int>(m_menuIndex))
	{
		//セレクトSE
		SoundManager::GetInstance().PlayOnceSE("Select");
	}
	menuIndex = MathSub::ClampInt(menuIndex, static_cast<int>(MenuIndex::Continue), static_cast<int>(MenuIndex::FinishGame));
	m_menuIndex = static_cast<MenuIndex>(menuIndex);
	//選ばれている項目に対応したUIに選ばれていることをフラグで伝える
	for (auto& menuUI : m_menuUIs)
	{
		//一度リセット
		menuUI.second.lock()->SetIsSelect(false);
	}
	//選ばれている項目のみtrue
	if (m_menuUIs[m_menuIndex].expired())return;
	m_menuUIs[m_menuIndex].lock()->SetIsSelect(true);
	auto& fader = Fader::GetInstance();
	//決定した時インデックスから処理を分岐
	if (input.IsTrigger("A") && m_titleUI.lock()->IsAppered() && !fader.IsFadeNow())
	{
		//決定SE
		SoundManager::GetInstance().PlayOnceSE("Decide");
		switch (m_menuIndex)
		{
		case MenuIndex::Continue:
			InitContinue();
			break;
		case MenuIndex::NewGame:
			InitNewGame();
			break;
		case MenuIndex::Option:
			Option();
			return;
			break;
		case MenuIndex::FinishGame:
			InitFinishGame();
			break;
		}
		m_update = &TitleScene::UpdateDialog;
		return;
	}
}

void TitleScene::InitContinue()
{
	//拡大した状態で待機
	if (m_menuUIs[m_menuIndex].expired())return;
	m_menuUIs[m_menuIndex].lock()->SetIsWait(true);
	//ダイアログを出す
	if (m_dialogUI.expired())return;
	m_dialogUI.lock()->SetIsDraw(true);
	m_dialogUI.lock()->SetText(L"続きから始めますか?");
	m_dialogUI.lock()->SelectYes();
}

void TitleScene::Continue()
{
	auto& fader = Fader::GetInstance();
	//だんだん暗く
	fader.FadeOut(kFadeOutSpeed);
	m_isDecide = true;
}

void TitleScene::InitNewGame()
{
	//拡大した状態で待機
	if (m_menuUIs[m_menuIndex].expired())return;
	m_menuUIs[m_menuIndex].lock()->SetIsWait(true);
	//ダイアログを出す
	if (m_dialogUI.expired())return;
	m_dialogUI.lock()->SetIsDraw(true);
	m_dialogUI.lock()->SetText(L"データを削除して\n最初から始めますか?");
	m_dialogUI.lock()->SelectYes();
}

void TitleScene::NewGame()
{
	auto& fader = Fader::GetInstance();
	//だんだん暗く
	fader.FadeOut(kFadeOutSpeed);
	m_isDecide = true;
	//データを削除
	SaveDataManager::GetInstance().NewGame();
}

void TitleScene::Option()
{
	m_controller.PushScene(std::make_shared<OptionScene>(m_controller));
	return;
}

void TitleScene::InitFinishGame()
{
	//拡大した状態で待機
	if (m_menuUIs[m_menuIndex].expired())return;
	m_menuUIs[m_menuIndex].lock()->SetIsWait(true);
	//ダイアログを出す
	if (m_dialogUI.expired())return;
	m_dialogUI.lock()->SetIsDraw(true);
	m_dialogUI.lock()->SetText(L"ゲームを終了しますか?");
	m_dialogUI.lock()->SelectYes();
}

void TitleScene::FinishGame()
{
	//アプリケーションの終了
	Application::GetInstance().FinishApplication();
}

void TitleScene::InitLight()
{
	m_lightHandle = CreateDirLightHandle(kLightDir);
}

void TitleScene::InitShadow()
{
	//シャドウマップハンドルの作成
	m_shadowMapHandle = MakeShadowMap(kShadowMapWidth, kShadowMapHeight);
}

void TitleScene::UpdateShadow()
{
	auto shadowMinPos = Vector3::Zero();
	shadowMinPos.x = -kShadowMapHorizon;
	shadowMinPos.y = kShadowMapVerticalMin;
	shadowMinPos.z = -kShadowMapHorizon;
	auto shadowMaxPos = Vector3::Zero();
	shadowMaxPos.x = kShadowMapHorizon;
	shadowMaxPos.y = kShadowMapVerticalMax;
	shadowMaxPos.z = kShadowMapHorizon;
	SetShadowMapDrawArea(m_shadowMapHandle, shadowMinPos.ToDxLibVector(), shadowMaxPos.ToDxLibVector());
}

void TitleScene::UpdateCommon()
{
	//カメラ更新
	m_camera->Update();
	//プレイヤー更新
	m_player->Update(m_isDecide);
	//ステージ更新
	for (auto& obj : m_stageObjects)
	{
		obj->Update();
	}
	//影
	UpdateShadow();
}

void TitleScene::InitUIs(std::shared_ptr<CSVDataLoader>& csvLoader)
{
	auto& uiManager = UIManager::GetInstance();
	auto titleUI = std::make_shared<TitleUI>();
	titleUI->Init();
	m_titleUI = titleUI;
	//メニューUI
	auto menuData = csvLoader->LoadUIDataCSV("Data/CSV/TitleMenuUITransformData.csv");
	for (int i = 0;i < menuData.size();++i)
	{
		int handle = -1;
		switch (static_cast<MenuIndex>(i))
		{
		case MenuIndex::Continue:
			handle = uiManager.GetImageHandle("ContinueT");
			break;
		case MenuIndex::NewGame:
			handle = uiManager.GetImageHandle("NewGame");
			break;
		case MenuIndex::Option:
			handle = uiManager.GetImageHandle("Option");
			break;
		case MenuIndex::FinishGame:
			handle = uiManager.GetImageHandle("FinishGame");
			break;
		default:
			break;
		}
		auto menuUI = std::make_shared<MenuUI>(menuData[i].pos, handle);
		menuUI->Init();
		m_menuUIs[static_cast<MenuIndex>(i)] = menuUI;
	}
	//セーブデータ
	auto saveDataUI = std::make_shared<SaveDataUI>();
	saveDataUI->Init();
	m_saveDataUI = saveDataUI;
	//ダイアログ
	auto dialogUI = std::make_shared<DialogUI>();
	dialogUI->Init();
	m_dialogUI = dialogUI;
}

void TitleScene::InitTitle()
{
	//メニューUIの非表示
	for (auto& menuUI : m_menuUIs)
	{
		menuUI.second.lock()->SetIsDraw(false);
	}
	//タイトルを表示とリセット
	m_titleUI.lock()->SetIsDraw(true);
	m_titleUI.lock()->Reset();
	//セーブデータを非表示
	m_saveDataUI.lock()->SetIsDraw(false);
	//ダイアログを非表示
	m_dialogUI.lock()->SetIsDraw(false);
	//次の状態に
	m_update = &TitleScene::UpdateTitle;
}

void TitleScene::InitSelectMenu()
{
	//メニューUIの表示とリセット
	for (auto& menuUI : m_menuUIs)
	{
		menuUI.second.lock()->SetIsDraw(true);
	}
	//待機解除
	if (m_menuUIs[m_menuIndex].expired())return;
	m_menuUIs[m_menuIndex].lock()->SetIsWait(false);
	//タイトルを非表示
	if (m_titleUI.expired())return;
	m_titleUI.lock()->SetIsDraw(false);
	//セーブデータを表示
	if (m_saveDataUI.expired())return;
	m_saveDataUI.lock()->SetIsDraw(true);
	//ダイアログを非表示
	if (m_dialogUI.expired())return;
	m_dialogUI.lock()->SetIsDraw(false);
	//次の状態に
	m_update = &TitleScene::UpdateSelectMenu;
}
