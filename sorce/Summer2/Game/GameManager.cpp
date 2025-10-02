#include "GameManager.h"
#include "../General/Input.h"
#include <DxLib.h>
#include "../General/game.h"
#include "Actors/ActorManager.h"
#include "Actors/Character/Player/Player.h"
#include "Actors/Character/Enemy/EnemyBase.h""
#include "UI/UIManager.h"
#include "../General/Collision/Physics.h"
#include "../Game/Camera/GameCamera/GameCamera.h"
#include "../Game/GameRule/Score.h"
#include "../Game/GameRule/Timer.h"
#include "../Game/UI/GameScoreUI.h"
#include "../Game/UI/TimerUI.h"
#include "../Game/UI/TutorialUI.h"
#include "../General/Effect/EffekseerManager.h"
#include "TutorialDirecter.h"
#include "../SaveData/SaveDataManager.h"
#include <cassert>

namespace
{
	//シャドウマップの描画サイズ
	constexpr int kShadowMapWidth = 8192;
	constexpr int kShadowMapHeight = 8192;
	//ライトの向き
	const VECTOR kLightDir = { 0.1f, -1.0f, 0.5f };
	//const VECTOR kLightDir2 = { 0.5f, 1.0f, 0.0f };
	//シャドウマップの範囲
	constexpr float kShadowMapHorizon = 5000.0f;
	constexpr float kShadowMapVerticalMin = -1.0f;
	constexpr float kShadowMapVerticalMax = 1000.0f;
}

GameManager::GameManager():
	m_isGameover(false),
	m_isGameClear(false),
	m_isResult(false),
	m_shadowMapHandle(-1)
{
	//カメラの初期化
	m_camera = std::make_shared<GameCamera>();
	//アクターマネージャー
	m_actorManager = std::make_shared<ActorManager>(m_camera);
	//タイマー
	m_timer = std::make_shared<Timer>();
}


GameManager::~GameManager()
{
}

void GameManager::Init(Stage::StageIndex index)
{
	//タイマーの初期化
	m_timer->Init();
	//アクターマネージャーの初期化
	m_actorManager->Init(index);
	//カメラの初期化
	m_camera->Init(index);
	auto& saveDataManager = SaveDataManager::GetInstance();
	auto score = saveDataManager.GetScore();
	//スコアの初期化
	if (!score.expired())score.lock()->Init();
	//UI作成
	UIManager::GetInstance().CreateGameScoreUI(score);
	UIManager::GetInstance().CreateTimerUI(m_timer);

	//ステージ1ならチュートリアル(かつ未クリアなら)
	if (index == Stage::StageIndex::Stage1 && !saveDataManager.IsClearStage(Stage::StageIndex::Stage1))
	{
		m_tutorialDirecter = std::make_shared<TutorialDirecter>();
	}

	//ライト
	InitLight();
	//シャドウマップの準備
	InitShadow();
	
}

void GameManager::Update()
{
	auto& saveDataManager = SaveDataManager::GetInstance();
	//デバッグで一時停止されてないなら
	auto& input = Input::GetInstance();
#if _DEBUG
	if (input.IsTrigger("StopUpdate"))
	{
		m_isUpdateStop = !m_isUpdateStop;
	}
	if (m_isUpdateStop)
	{
		//更新を止める
		Physics::GetInstance().StopUpdate();
	}
	if (!m_isUpdateStop || input.IsTrigger("OneFrame"))
#endif
	{
		//更新を再開
		Physics::GetInstance().StartUpdate();
		//アクターの更新
		m_actorManager->Update(saveDataManager.GetScore());
		//ゲーム開始時の処理
		UpdateGameStart();
		//チュートリアル
		if (m_tutorialDirecter != nullptr)
		{
			m_tutorialDirecter->Update(m_actorManager);
		}
		//プレイヤー
		if (m_actorManager->GetPlayer().expired())return;
		auto player = m_actorManager->GetPlayer().lock();
		//ボスを倒したとき
		if (m_actorManager->IsBossDead())
		{
			//タイマーを止める
			m_timer->StopUpdate();
			//UIの描画を止める
			UIManager::GetInstance().StopDraw();
			//クリアしていないなら
			if (!m_isGameClear)input.StopUpdate();//入力を止める
			else input.StartUpdate();//入力を開始
			//プレイヤーの勝利アニメーションをしたとき
			if (player->IsClearAnim())
			{
				//クリア
				m_isGameClear = true;
			}
			//プレイヤーの勝利アニメーションが終了したら
			if (player->IsFinishClearAnim() && m_isGameClear)
			{
				//タイマーをスコアに加算
				if (saveDataManager.GetScore().expired())return;
				saveDataManager.GetScore().lock()->AddTimeScore(m_timer->GetTime());
				//リザルトへ
				m_isResult = true;
			}
		}
		//プレイヤーが死亡した際の処理
		else if (player->IsDead() && !m_isGameover)
		{
			//UIの非表示
			UIManager::GetInstance().AllStopDraw();
			//ゲームオーバー
			m_isGameover = true;
		}
		//タイマー
		m_timer->Update();
		//カメラの更新
		m_camera->Update(m_actorManager);
	}
	//シャドウマップに描画する範囲を設定
	//カメラの周囲のみ
	UpdateShadowDrawArea();
}

void GameManager::Draw() const
{
#if _DEBUG
	DrawString(0, 0, L"Stage1 Scene", 0xffffff);
	DrawString(0, 16, L"[D]キーで Debug Scene", 0xffffff);

	//for (int z = -500; z <= 500; z += 100)
	//{
	//	DrawLine3D(VGet(-500, 0, z), VGet(500, 0, z), 0xff0000);
	//}
	//for (int x = -500; x <= 500; x += 100)
	//{
	//	DrawLine3D(VGet(x, 0, -500), VGet(x, 0, 500), 0x0000ff);
	//}
	//VECTOR screenPos = ConvWorldPosToScreenPos(VGet(500, 0, 0));
	//DrawString(screenPos.x, screenPos.y, L"X+", 0xffffff);
	//screenPos = ConvWorldPosToScreenPos(VGet(-500, 0, 0));
	//DrawString(screenPos.x, screenPos.y, L"X-", 0xffffff);
	//screenPos = ConvWorldPosToScreenPos(VGet(0, 0, 500));
	//DrawString(screenPos.x, screenPos.y, L"Z+", 0xffffff);
	//screenPos = ConvWorldPosToScreenPos(VGet(0, 0, -500));
	//DrawString(screenPos.x, screenPos.y, L"Z-", 0xffffff);
#endif
	if (m_shadowMapHandle >= 0)
	{
		//シャドウマップが想定するライトの方向もセット
		SetShadowMapLightDirection(m_shadowMapHandle, kLightDir);
		//シャドウマップへの描画の準備
		ShadowMap_DrawSetup(m_shadowMapHandle);
		//シャドウマップへのアクターの描画
		m_actorManager->Draw();
		//シャドウマップへの描画を終了
		ShadowMap_DrawEnd();
		//描画に使用するシャドウマップを設定
		SetUseShadowMap(0, m_shadowMapHandle);
		//アクターの描画
		m_actorManager->Draw();
		//描画に使用するシャドウマップの設定を解除
		SetUseShadowMap(0, -1);
	}
}

void GameManager::End()
{
	//アクターマネージャーの終了
	m_actorManager->End();
	//UIマネージャーのリセット
	UIManager::GetInstance().Reset();
	//エフェクトをリセット
	EffekseerManager::GetInstance().Reset();
	//ライトの削除
	for (auto& handle : m_lightHandles)
	{
		DeleteLightHandle(handle);
	}
	//シャドウマップの削除
	DeleteShadowMap(m_shadowMapHandle);
	//チュートリアル終了処理
	if (m_tutorialDirecter != nullptr)
	{
		m_tutorialDirecter->End();
	}
}

void GameManager::Restart(Stage::StageIndex index)
{
	//タイマーの初期化
	m_timer->Init();
	//UIマネージャーのリセット
	UIManager::GetInstance().Reset();
	//エフェクトをリセット
	EffekseerManager::GetInstance().Reset();
	//アクターマネージャーの再スタート
	m_actorManager->Restart();
	//カメラの初期化
	m_camera->Init(index);
	//フラグリセット
	m_isGameover = false;
	m_isGameClear = false;
	auto& saveDataManager = SaveDataManager::GetInstance();
	auto score = saveDataManager.GetScore();
	//スコアの初期化
	if(!score.expired())score.lock()->Init();
	//UI作成
	UIManager::GetInstance().CreateGameScoreUI(score);
	UIManager::GetInstance().CreateTimerUI(m_timer);
}
void GameManager::Continue()
{
	//スコアの初期化
	SaveDataManager::GetInstance().GetScore().lock()->Init();
	//プレイヤーを復活
	m_actorManager->RevivalPlayer();
	//UIの描画を止める
	UIManager::GetInstance().AllStartDraw();
	//フラグリセット
	m_isGameover = false;
	m_isGameClear = false;
	m_isResult = false;
}

void GameManager::InitLight()
{
	m_lightHandles.emplace_back(CreateDirLightHandle(kLightDir));
	//m_lightHandles.emplace_back(CreateDirLightHandle(kLightDir2));
}

void GameManager::InitShadow()
{
	//シャドウマップハンドルの作成
	m_shadowMapHandle = MakeShadowMap(kShadowMapWidth, kShadowMapHeight);
}

void GameManager::UpdateShadowDrawArea()
{
	auto shadowMinPos = m_camera->GetPos();
	shadowMinPos.x += -kShadowMapHorizon;
	shadowMinPos.y = kShadowMapVerticalMin;
	shadowMinPos.z += -kShadowMapHorizon;
	auto shadowMaxPos = m_camera->GetPos();
	shadowMaxPos.x += kShadowMapHorizon;
	shadowMaxPos.y = kShadowMapVerticalMax;
	shadowMaxPos.z += kShadowMapHorizon;
	SetShadowMapDrawArea(m_shadowMapHandle, shadowMinPos.ToDxLibVector(), shadowMaxPos.ToDxLibVector());
}

void GameManager::UpdateGameStart()
{
	if (!m_actorManager->GetPlayer().expired())
	{
		//プレイヤーがスタート状態の時
		if (m_actorManager->GetPlayer().lock()->IsStartAnim())
		{
			//タイマーを止める
			m_timer->StopUpdate();
		}
		//プレイヤーがスタート状態が終了したら
		else
		{
			//タイマーを開始
			m_timer->StartUpdate();
		}
	}
}