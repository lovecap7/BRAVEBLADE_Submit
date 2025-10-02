#include "TitleUI.h"
#include "../../../General/game.h"
#include "../../../General/Input.h"
#include "../../../Shader/MyDraw.h"
#include <DxLib.h>
#include <assert.h>

namespace
{
	//タイトル
	constexpr float kTitlePosX = Game::kScreenWidth / 2;
	constexpr float kTitlePosY = 250;
	//PAB
	constexpr float kPressAnyBottunPosX = Game::kScreenWidth / 2;
	constexpr float kPressAnyBottunPosY = 600;
	//点滅全体フレーム
	constexpr int kBlinkingFrame = 60;
	//非表示
	constexpr int kShutFrame = kBlinkingFrame / 2;
	//ディゾルブの速度
	constexpr float kDissolveSpeed = 0.01f;
}

TitleUI::TitleUI() :
	DissolveUIBase(LoadGraph(L"Data/UI/Title/Title.png"), LoadPixelShader(L"Shader/DissolvePS.pso"),LoadGraph(L"Data/ShaderSample/pattern6.png"),false),
	m_countFrame(0),
	m_titleBackHandle(LoadGraph(L"Data/UI/Title/TitleBack.png")),
	m_pressAnyBottunHandle(LoadGraph(L"Data/UI/Title/PressAnyBottun.png")),
	m_pressAnyBottunBackHandle(LoadGraph(L"Data/UI/Title/PressAnyBottunBack.png")),
	m_isDecide(false),
	m_isAppeared(false)
{
	assert(m_titleBackHandle != -1);
	assert(m_pressAnyBottunHandle != -1);
	assert(m_pressAnyBottunBackHandle != -1);
	assert(m_patternHandle != -1);
}

TitleUI::~TitleUI()
{
	DeleteGraph(m_titleBackHandle);
	DeleteGraph(m_pressAnyBottunHandle);
	DeleteGraph(m_pressAnyBottunBackHandle);
	DeleteGraph(m_patternHandle);
}

void TitleUI::Update()
{
	//だんだんタイトルが出現する
	DissolveUIBase::Update(kDissolveSpeed, true);
	//完全に出現したら
	if (DissolveUIBase::IsAppered())
	{
		m_isAppeared = true;
		++m_countFrame;
		if (m_countFrame > kBlinkingFrame)
		{
			m_countFrame = 0;
		}
	}
}

void TitleUI::Draw() const
{
	if (!m_isDraw)return;
	//決定したら少し光る
	if (m_isDecide)
	{
		DrawRotaGraph(kTitlePosX, kTitlePosY, 1.0, 0.0, m_titleBackHandle, true);
	}
	//シェーダで描画
	DissolveUIBase::Draw(Vector2{ kTitlePosX ,kTitlePosY });

	if (m_isDecide)return;
	//点滅
	if (m_countFrame <= kShutFrame && m_isAppeared)
	{
		DrawRotaGraph(kPressAnyBottunPosX, kPressAnyBottunPosY, 1.0, 0.0, m_pressAnyBottunBackHandle, true);
		DrawRotaGraph(kPressAnyBottunPosX, kPressAnyBottunPosY, 1.0, 0.0, m_pressAnyBottunHandle, true);
	}
}
void TitleUI::Reset()
{
	m_countFrame = 0;
	m_isDecide = false;
	m_isAppeared = false;
}
