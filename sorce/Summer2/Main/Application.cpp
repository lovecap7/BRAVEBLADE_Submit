#include "Application.h"
#include <DxLib.h>
#include "../General/game.h"
#include "../Scene/SceneController.h"
#include  "../General/Input.h"
#include  "../General/Collision/Physics.h"
#include  "../General/Effect/EffekseerManager.h"
#include  "../Game/UI/UIManager.h"
#include  "../General/Fader.h"
#include  "../General/Sound/SoundManager.h"
#include  "../SaveData/SaveDataManager.h"
#include  "../General/LoadingManager.h"
#include  <cassert>

Application& Application::GetInstance()
{
    //この時点でメモリが確保されてアプリ終了まで残る
    static Application app;
    return app;
}

bool Application::Init()
{
	//初期化
	//このフラグがtrueの時アプリケーションが終了する
	m_isFinishApplication = false;

	//ゲームタイトル
	SetWindowText(L"BRAVE BLADE");
	//ゲームアイコン
	//SetWindowIconID(IDI_ICON1);

	 //フルスクリーンでなく、ウィンドウモードで開くようにする
	//こういった関数はウィンドウが開く前に(Dxlib.Init()の前)に処理しておく必要がある
	ChangeWindowMode(m_isWindow);

	// DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	//画面サイズ変更
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return false;			// エラーが起きたら直ちに終了
	}

	//描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	//3D描画のための準備
	//Zバッファを使用する
	SetUseZBuffer3D(true);
	//Zバッファに書き込みを行う
	SetWriteZBuffer3D(true);
	//バックカリングを行う(ポリゴンの裏面を描画しないようにする)
	SetUseBackCulling(true);

	//Physicsの初期化
	Physics::GetInstance().Init();
	//エフェクトの初期化
	EffekseerManager::GetInstance().Init();

    return true;
}

void Application::Run()
{
	//ローディング
	auto& loadingManager = LoadingManager::GetInstance();
	//非同期ロード開始
	loadingManager.StartLoading();
	//コントローラー
	auto& input = Input::GetInstance();
	input.Init();
	//Physics(衝突処理)
	auto& physics = Physics::GetInstance();
	//セーブデータ
	auto& saveDataManager = SaveDataManager::GetInstance();
	saveDataManager.Init();
	//サウンド
	auto& soundManager = SoundManager::GetInstance();
	soundManager.Init();
	//エフェクト
	auto& effect = EffekseerManager::GetInstance();
	//UIマネージャー
	auto& uiManager = UIManager::GetInstance();
	uiManager.Init();
	//フェード
	auto& fader = Fader::GetInstance();
	fader.Init();
	//シーン
	SceneController* sceneController = new SceneController();
	//ローディング初期化
	loadingManager.Init();
	//非同期終了
	loadingManager.StopLoading();

	//ゲームループ
	while (ProcessMessage() != -1) // Windowsが行う処理を待つ
	{
		//今回のループが始まった時間を覚えておく
		LONGLONG time = GetNowHiPerformanceCount();

		//画面全体をクリア
		ClearDrawScreen();

		//ここにゲームの処理を書く
		//Widowモードが切り替わったかをチェック
		bool isWindow = m_isWindow;
		
		//更新
		//ロード中は更新を止める
		bool isLoading = loadingManager.IsLoading();
		soundManager.Update();
		saveDataManager.Update();
		if (!isLoading)
		{
			input.Update();
			sceneController->Update();
			physics.Update();
			effect.Update();
			uiManager.Update();
			fader.Update();
		}
		loadingManager.Update();
		//ロード中は描画を止める
		if (!isLoading)
		{
			uiManager.BackDraw();
			sceneController->Draw();
			effect.Draw();
			uiManager.FrontDraw();
			fader.Draw();
		}
		//描画
		loadingManager.Draw();

#if _DEBUG
		DrawFormatString(0, 500, 0xff0000, L"FPS : %.2f", GetFPS());
#endif
		//切り替わったなら
		if (m_isWindow != isWindow)
		{
			//切り替わり処理
			ChangeScreenMode();
		}

		//画面の切り替わりを待つ必要がある
		ScreenFlip();//1/60秒経過するまで待つ

		//FPSを60に固定
		while (GetNowHiPerformanceCount() - time < 16667);
		{

		}

		//ESCキーで終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) || m_isFinishApplication)
		{
			sceneController = nullptr;
			break;
		}
	}
	delete sceneController;
	sceneController = nullptr;
}

void Application::Terminate()
{
	Physics::GetInstance().Reset();
	EffekseerManager::GetInstance().End();
	UIManager::GetInstance().End();
	SoundManager::GetInstance().End();
	SaveDataManager::GetInstance().End();
	LoadingManager::GetInstance().End();
	DxLib_End();				// ＤＸライブラリ使用の終了処理
}

void Application::SetWindowMode(bool isWindow)
{
	m_isWindow = isWindow;
}

void Application::ChangeScreenMode()
{
	//画面モード変更時(とウインドウモード変更時 )にグラフィックスシステムの設定やグラフィックハンドルをリセットするかどうかを設定する
	//Flag TRUE:リセットする(デフォルト)FALSE:リセットしない
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	//解像度
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum);
	//切り替え
	ChangeWindowMode(m_isWindow);
	// 実際のウィンドウサイズも設定（フレーム込み）
	SetWindowSize(Game::kScreenWidth, Game::kScreenHeight);
	//て拡大率を1倍に戻す
	SetWindowSizeExtendRate(1.0);
	//画面全体をクリア
	ClearDrawScreen();
}
