#include "SelectStageBackUI.h"
#include "../../../General/game.h"
#include <DxLib.h>

namespace
{
	constexpr float kDissolveSpeed = 0.05f;
}

SelectStageBackUI::SelectStageBackUI(int mainHandle):
	DissolveUIBase(mainHandle, LoadPixelShader(L"Shader/DissolveSlidePS.pso"), LoadGraph(L"Data/ShaderSample/pattern6.png"), true),
	m_isAppear(true)
{
	//後ろ描画
	m_isFront = false;
}

SelectStageBackUI::~SelectStageBackUI()
{
}

void SelectStageBackUI::Update()
{
	DissolveUIBase::Update(kDissolveSpeed, m_isAppear);
}

void SelectStageBackUI::Draw() const
{
	//完全に出現してるとき
	if (IsAppered())
	{
		//シェーダー画像の下に描画したときに透明部分から見えるように
		DrawRotaGraph(Game::kScreenCenterX, Game::kScreenCenterY, 1.0, 0.0, m_mainHandle, true);
	}
	else
	{
		if (IsDisappeared())
		{
			return;
		}
		//シェーダー
		DissolveUIBase::Draw(Vector2{ Game::kScreenCenterX,Game::kScreenCenterY });
	}
}
