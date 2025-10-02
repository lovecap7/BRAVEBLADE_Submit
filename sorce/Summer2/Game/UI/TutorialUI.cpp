#include "TutorialUI.h"
#include "UIManager.h"
#include "../../General/game.h"
#include "../../General/StringUtil.h"
#include "../../General/Math/MyMath.h"
#include <DxLib.h>

namespace
{
	//四角の座標
	constexpr int kSpaceLeftPos = Game::kScreenCenterX - 100;
	constexpr int kSpaceUpPos = 60;
	constexpr int kSpaceRightPos = Game::kScreenWidth;
	constexpr int kSpaceDownPos = 135;
	//改行の数だけ四角の下の座標を下げてく
	constexpr int kSpaceDownSize = 20;
	//”チュートリアル”の表示位置
	constexpr int kTutorialOffsetPosX = 50;
	constexpr int kTutorialPosY = kSpaceUpPos + 10;
	//１行に入る文字列の数
	constexpr int kOneLineMaxNum = 30;
	//テキストの座標
	constexpr int kTextOffsetPosX = 50;
	constexpr int kTextPosY = kTutorialPosY + 30;
	//テキストの表示速度
	constexpr int kTextViewSpeed = 2;
	//生存時間
	constexpr int kAppearInterval = 20;
	constexpr int kNormalInterval = 10 * 60;
	//透明度
	constexpr float kAlphaRate = 200;
	//枠線の太さ
	constexpr int kFrameThickness = 10;
	//チュートリアルマークの座標
	constexpr int kTutorialMarkPosX = 20;
	constexpr int kTutorialMarkPosY = kSpaceUpPos + 40;
}

TutorialUI::TutorialUI(const std::wstring& text):
	UIBase(),
	m_countFrame(0),
	m_chatCount(0),
	m_text{ text },
	m_update(&TutorialUI::AppearUpdate),
	m_posX(Game::kScreenWidth),
	m_textHandle(UIManager::GetInstance().GetTextHandle("源直ゴシック20")),
	m_tutorialMarkUI(UIManager::GetInstance().GetImageHandle("TutorialMark"))
{
	m_text = StringUtil::InsertNewLines(m_text, kOneLineMaxNum);
}

TutorialUI::~TutorialUI()
{
}

void TutorialUI::Update()
{
	(this->*m_update)();
}

void TutorialUI::Draw() const
{
	//チュートリアルのスペース
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlphaRate);
	//改行の数
	auto lineNum = StringUtil::WStringLineNum(m_text);
	DrawBox(m_posX - kFrameThickness, kSpaceUpPos - kFrameThickness, kSpaceRightPos, kSpaceDownPos + kSpaceDownSize * lineNum + kFrameThickness, 0xffffff, true);
	DrawBox(m_posX, kSpaceUpPos, kSpaceRightPos, kSpaceDownPos + kSpaceDownSize * lineNum, 0x444444, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//テキスト
	DrawStringToHandle(m_posX + kTutorialOffsetPosX, kTutorialPosY, L"<チュートリアル>", 0xffffff, m_textHandle);
	DrawStringToHandle(m_posX + kTextOffsetPosX, kTextPosY, m_text.substr(0, m_chatCount).c_str(), 0xffffff, m_textHandle);
	//チュートリアルマーク
	DrawRotaGraph(m_posX + kTutorialMarkPosX, kTutorialMarkPosY, 1.0, 0.0, m_tutorialMarkUI, true);
}

void TutorialUI::AppearUpdate()
{
	m_posX = MathSub::Lerp(m_posX, kSpaceLeftPos, 0.1f);
	++m_countFrame;
	if (m_countFrame > kAppearInterval)
	{
		m_countFrame = 0;
		m_update = &TutorialUI::NormalUpdate;
		return;
	}
}

void TutorialUI::NormalUpdate()
{
	++m_countFrame;
	//表示フレーム
	if (m_countFrame > kNormalInterval)
	{
		m_countFrame = kAppearInterval;
		m_update = &TutorialUI::DisappearUpdate;
		return;
	}
	//テキストを進める
	if (m_countFrame % kTextViewSpeed == 0 && m_chatCount < static_cast<int>(m_text.size()))
	{
		++m_chatCount;
	}
}

void TutorialUI::DisappearUpdate()
{
	m_posX = MathSub::Lerp(m_posX, Game::kScreenWidth, 0.1f);
	--m_countFrame;
	if (m_countFrame < 0)
	{
		m_isDelete = true;
		return;
	}
}
