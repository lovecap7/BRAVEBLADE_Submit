#include "DialogUI.h"
#include "UIManager.h"
#include "../../General/StringUtil.h"
#include "../../General/game.h"
#include "MenuUI.h"

namespace
{
	constexpr int kTextPosX = Game::kScreenCenterX;
	constexpr int kTextPosY = Game::kScreenCenterY - 110;
	//改行文字数
	constexpr int kLineTextNum = 11;
}

DialogUI::DialogUI():
	UIBase()
{
	auto& uiManager = UIManager::GetInstance();
	m_backHandle = uiManager.GetImageHandle("Dialog");
	m_texthandle = uiManager.GetTextHandle("源直ゴシック48");
}

DialogUI::~DialogUI()
{
}

void DialogUI::Init()
{
	auto& uiManager = UIManager::GetInstance();
	//UIManagerに登録
	uiManager.Entry(shared_from_this());
	//Yes
	auto yes = std::make_shared<MenuUI>(Vector2{ Game::kScreenCenterX - 200, Game::kScreenCenterY + 150 }, uiManager.GetImageHandle("Yes"));
	yes->Init();
	m_yesUI = yes;
	//No
	auto no = std::make_shared<MenuUI>(Vector2{ Game::kScreenCenterX + 200, Game::kScreenCenterY + 150 }, uiManager.GetImageHandle("No"));
	no->Init();
	m_noUI = no;
}

void DialogUI::Update()
{
	//表示
	m_yesUI.lock()->SetIsDraw(m_isDraw);
	m_noUI.lock()->SetIsDraw(m_isDraw);
}

void DialogUI::Draw() const
{
	if (!m_isDraw)return;
	//改行の数
	auto lineNum = StringUtil::WStringLineNum(m_text);
	//背景
	DrawRotaGraph(Game::kScreenCenterX, Game::kScreenCenterY, 1.0, 0.0, m_backHandle, true);
	//テキスト
	//文字列の長さを取得して中央ぞろえに

	//文字数
	int len = wcslen(m_text.c_str());
	int cLen = MathSub::ClampInt(len, 0, kLineTextNum);
	//文字の大きさ
	int size =  GetFontSizeToHandle(m_texthandle);
	//動かす幅
	int shiftX = (size * cLen) * 0.5f;
	DrawStringToHandle(kTextPosX - shiftX, kTextPosY, m_text.c_str(), 0xffffff, m_texthandle);
}

void DialogUI::SetText(const std::wstring& text)
{
	m_text = StringUtil::InsertNewLines(text, kLineTextNum);
}

void DialogUI::SelectYes()
{
	m_yesUI.lock()->SetIsSelect(true);
	m_noUI.lock()->SetIsSelect(false);
}

void DialogUI::SelectNo()
{
	m_yesUI.lock()->SetIsSelect(false);
	m_noUI.lock()->SetIsSelect(true);
}

bool DialogUI::IsYes()
{
	return m_yesUI.lock()->IsSelect();
}
