#include "UIManager.h"
#include "UIBase.h"
#include "../Actors/ActorManager.h"
#include <cassert>
//プレイヤー
#include "PlayerUI/PlayerHPUI.h"
#include "PlayerUI/PlayerUltGageUI.h"
//ボス
#include "EnemyUI/BossHPUI.h"
//敵
#include "EnemyUI/EnemyHPUI.h"
//スコア
#include "GameScoreUI.h"
//タイマー
#include "TimerUI.h"
#include <Windows.h>
namespace
{
	constexpr int kFontSize16 = 16;
	constexpr int kFontSize20 = 20;
	constexpr int kFontSize32 = 32;
	constexpr int kFontSize48 = 48;
	constexpr int kFontSize64 = 64;
	constexpr int kFontSize80 = 80;
	constexpr int kFontSize96 = 96;
}

void UIManager::Entry(std::shared_ptr<UIBase> ui)
{
	//すでに登録されているならしない
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it != m_uis.end())return;
	//登録
	m_uis.emplace_back(ui);
}

void UIManager::Exit(std::shared_ptr<UIBase> ui)
{
	//登録されていないならしない
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it == m_uis.end())return;
	m_uis.erase(it);
}
void UIManager::Init()
{
	//ハンドルロード
	LoadHandle();
	//更新と描画フラグ
	m_isUpdate = true;
	m_isDraw = true;
}

void UIManager::Update()
{
	//更新をするか
	if (!m_isUpdate)return;
	//削除予定のUI削除
	CheckDelete();
	//更新
	for (auto& ui : m_uis)
	{
		ui->Update();
	}
}

void UIManager::BackDraw() const
{
	//描画をするか
	if (!m_isDraw)return;
	//優先度低い順に描画
	for (auto& ui : m_uis)
	{
		//後ろ描画じゃないなら
		if (ui->IsFront())continue;
		if (ui->GetPriority() != UIBase::Priority::Low)continue;
		ui->Draw();
	}
	for (auto& ui : m_uis)
	{
		//後ろ描画じゃないなら
		if (ui->IsFront())continue;
		if (ui->GetPriority() != UIBase::Priority::Middle)continue;
		ui->Draw();
	}
	for (auto& ui : m_uis)
	{
		//後ろ描画じゃないなら
		if (ui->IsFront())continue;
		if (ui->GetPriority() != UIBase::Priority::High)continue;
		ui->Draw();
	}
	
}

void UIManager::FrontDraw() const
{
	//描画をするか
	if (!m_isDraw)return;
	//優先度低い順に描画
	for (auto& ui : m_uis)
	{
		//前描画じゃないなら
		if (!ui->IsFront())continue;
		if (ui->GetPriority() != UIBase::Priority::Low)continue;
		ui->Draw();
	}
	for (auto& ui : m_uis)
	{
		//前描画じゃないなら
		if (!ui->IsFront())continue;
		if (ui->GetPriority() != UIBase::Priority::Middle)continue;
		ui->Draw();
	}
	for (auto& ui : m_uis)
	{
		//前描画じゃないなら
		if (!ui->IsFront())continue;
		if (ui->GetPriority() != UIBase::Priority::High)continue;
		ui->Draw();
	}
}

void UIManager::End()
{
	//すべて削除
	AllDeleteUIs();
	//ハンドル削除
	AllDeleteHandle();
}

void UIManager::Reset()
{
	//UIのみすべて削除
	AllDeleteUIs();
}

void UIManager::CreatePlayerUI(const std::weak_ptr<Player> player)
{
	Entry(std::make_shared<PlayerHPUI>(player));
	Entry(std::make_shared<PlayerUltGageUI>(player));
}

std::weak_ptr<BossHPUI> UIManager::CreateBossUI(const std::weak_ptr<EnemyBase> boss)
{
	std::shared_ptr<BossHPUI> ui = std::make_shared<BossHPUI>(boss);
	Entry(ui);
	return ui;
}

void UIManager::CreateEnemyUI(const std::weak_ptr<EnemyBase> enemy)
{
	Entry(std::make_shared<EnemyHPUI>(enemy));
}

std::weak_ptr<GameScoreUI> UIManager::CreateGameScoreUI(const std::weak_ptr<Score> score)
{
	auto gameScoreUI = std::make_shared<GameScoreUI>(score);
	Entry(gameScoreUI);
	return gameScoreUI;
}

std::weak_ptr<TimerUI> UIManager::CreateTimerUI(const std::weak_ptr<Timer> timer)
{
	auto timerUI = std::make_shared<TimerUI>(timer);
	Entry(timerUI);
	return timerUI;
}

int UIManager::GetImageHandle(const std::string& name) const
{
	auto it = m_imageHandles.find(name);
	if (it == m_imageHandles.end()) {
		//ログを出すかデフォルト値を返す
		assert(false && "ImageHandles not found");
		return -1;
	}
	return it->second;
}

int UIManager::GetTextHandle(const std::string& name) const
{
	auto it = m_textHandles.find(name);
	if (it == m_textHandles.end()) {
		//ログを出すかデフォルト値を返す
		assert(false && "TextHandle not found");
		return -1;
	}
	return it->second;
}

void UIManager::LoadHandle()
{
	//フォントの読み込み
	LPCSTR fontPath = "Font/源直ゴシック EMG 3 - BOLD.ttf";
	if (AddFontResourceExA(fontPath, FR_PRIVATE, NULL) > 0)
	{
	}
	else
	{
		// フォント読込エラー処理
		assert(0 && "フォント読み込み失敗");
	}
	//画像ハンドル
	m_imageHandles["Score"] = { LoadGraph(L"Data/UI/Number.png") };
	m_imageHandles["Timer"] = { LoadGraph(L"Data/UI/Number_Stone.png") };
	m_imageHandles["WindowMode"] = { LoadGraph(L"Data/UI/Option/WindowMode.png") };
	m_imageHandles["FullScreenMode"] = { LoadGraph(L"Data/UI/Option/FullScreenMode.png") };
	m_imageHandles["SaveDataBack"] = { LoadGraph(L"Data/UI/SaveDataBack.png") };
	m_imageHandles["Dialog"] = { LoadGraph(L"Data/UI/Dialog.png") };
	m_imageHandles["Yes"] = { LoadGraph(L"Data/UI/Yes.png") };
	m_imageHandles["No"] = { LoadGraph(L"Data/UI/No.png") };
	m_imageHandles["PlayerHP"] = { LoadGraph(L"Data/UI/Player/PlayerHP.png") };
	m_imageHandles["PlayerGageFrame"] = { LoadGraph(L"Data/UI/Player/PlayerGageFrame.png") };
	m_imageHandles["PlayerUltGage"] = { LoadGraph(L"Data/UI/Player/PlayerUltGage.png") };
	m_imageHandles["PlayerMaxUltGage"] = { LoadGraph(L"Data/UI/Player/PlayerMaxUltGage.png") };
	m_imageHandles["UltBottun"] = { LoadGraph(L"Data/UI/Player/UltBottun.png") };
	m_imageHandles["EnemyHPFrame"] = { LoadGraph(L"Data/UI/Enemy/EnemyHPFrame.png") };
	m_imageHandles["EnemyHP"] = { LoadGraph(L"Data/UI/Enemy/EnemyHP.png") };
	m_imageHandles["BossHPFrame"] = { LoadGraph(L"Data/UI/Enemy/BossHPFrame.png") };
	m_imageHandles["BossHP"] = { LoadGraph(L"Data/UI/Enemy/BossHP.png") };
	m_imageHandles["TutorialMark"] = { LoadGraph(L"Data/UI/TutorialMark.png") };
	m_imageHandles["LeftArrow"] = { LoadGraph(L"Data/UI/Arrow.png") };
	m_imageHandles["LockLeftArrow"] = { LoadGraph(L"Data/UI/LockArrow.png") };
	m_imageHandles["SoundBottun"] = { LoadGraph(L"Data/UI/Option/SoundBottun.png") };
	m_imageHandles["OptionBack"] = { LoadGraph(L"Data/UI/Option/OptionBack.png") };
	//メニュー
	m_imageHandles["Pause"] = { LoadGraph(L"Data/UI/Menu/Pause.png") };
	m_imageHandles["Gameover"] = { LoadGraph(L"Data/UI/Menu/Gameover.png") };
	m_imageHandles["Continue"] = { LoadGraph(L"Data/UI/Menu/Continue.png") };
	m_imageHandles["ContinueT"] = { LoadGraph(L"Data/UI/Title/ContinueT.png") };
	m_imageHandles["ReturnGame"] = { LoadGraph(L"Data/UI/Menu/ReturnGame.png") };
	m_imageHandles["RestartGame"] = { LoadGraph(L"Data/UI/Menu/RestartGame.png") };
	m_imageHandles["Option"] = { LoadGraph(L"Data/UI/Menu/Option.png") };
	m_imageHandles["SelectStage"] = { LoadGraph(L"Data/UI/Menu/SelectStage.png") };
	m_imageHandles["NewGame"] = { LoadGraph(L"Data/UI/Title/NewGame.png") };
	m_imageHandles["FinishGame"] = { LoadGraph(L"Data/UI/Title/FinishGame.png") };
	m_imageHandles["1st"] = { LoadGraph(L"Data/UI/Result/1st.png") };
	m_imageHandles["2nd"] = { LoadGraph(L"Data/UI/Result/2nd.png") };
	m_imageHandles["3rd"] = { LoadGraph(L"Data/UI/Result/3rd.png") };
	//ロードに成功したかチェック
	for (auto& [key, value] : m_imageHandles)
	{
		assert(value >= 0);
	}
	//テキストハンドル
	m_textHandles["メイリオ16"] = { CreateFontToHandle(L"Meiryo", kFontSize16, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["メイリオ32"] = { CreateFontToHandle(L"Meiryo", kFontSize32, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["メイリオ48"] = { CreateFontToHandle(L"Meiryo", kFontSize48, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["メイリオ64"] = { CreateFontToHandle(L"Meiryo", kFontSize64, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["メイリオ80"] = { CreateFontToHandle(L"Meiryo", kFontSize80, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["メイリオ96"] = { CreateFontToHandle(L"Meiryo", kFontSize96, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["MSPゴシック16"] = { CreateFontToHandle(L"MS PGothic", kFontSize16, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["MSPゴシック32"] = { CreateFontToHandle(L"MS PGothic", kFontSize32, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["MSPゴシック48"] = { CreateFontToHandle(L"MS PGothic", kFontSize48, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["MSPゴシック64"] = { CreateFontToHandle(L"MS PGothic", kFontSize64, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["源直ゴシック16"] = { CreateFontToHandle(L"源直ゴシック EMG 3 - BOLD", kFontSize16, 5, DX_FONTTYPE_ANTIALIASING_EDGE) };
	m_textHandles["源直ゴシック20"] = { CreateFontToHandle(L"源直ゴシック EMG 3 - BOLD", kFontSize20, 5, DX_FONTTYPE_ANTIALIASING_EDGE) };
	m_textHandles["源直ゴシック32"] = { CreateFontToHandle(L"源直ゴシック EMG 3 - BOLD", kFontSize32, 5, DX_FONTTYPE_ANTIALIASING_EDGE) };
	m_textHandles["源直ゴシック48"] = { CreateFontToHandle(L"源直ゴシック EMG 3 - BOLD", kFontSize48, 5, DX_FONTTYPE_ANTIALIASING_EDGE) };
	//ロードに成功したかチェック
	for (auto& [key, value] : m_textHandles) {
		assert(value >= 0);
	}
}

void UIManager::AllStopDraw()
{
	for (auto& ui : m_uis)
	{
		ui->SetIsDraw(false);
	}
}

void UIManager::AllStartDraw()
{
	for (auto& ui : m_uis)
	{
		ui->SetIsDraw(true);
	}
}

void UIManager::AllDeleteUIs()
{
	m_uis.clear();
}

void UIManager::AllDeleteHandle()
{
	//画像の削除
	for (auto& [key, value] : m_imageHandles) {
		if (value >= 0)
		{
			auto result = DeleteGraph(value);
			assert(result == 0);
		}
	}
	m_imageHandles.clear();
	//テキストの削除
	for (auto& [key, value] : m_textHandles) {
		DeleteFontToHandle(value);
	}
	m_textHandles.clear();
	// ウィンドウズに一時的に保持していたフォントデータを削除
	RemoveFontResourceExA("", FR_PRIVATE, NULL);
}

void UIManager::CheckDelete()
{
	auto remIt = std::remove_if(m_uis.begin(), m_uis.end(), [](std::shared_ptr<UIBase> ui) {
		return  ui->IsDelete();
		});
	m_uis.erase(remIt, m_uis.end());//削除
}
