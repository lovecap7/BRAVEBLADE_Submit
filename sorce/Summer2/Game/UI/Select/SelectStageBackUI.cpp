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
	//���`��
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
	//���S�ɏo�����Ă�Ƃ�
	if (IsAppered())
	{
		//�V�F�[�_�[�摜�̉��ɕ`�悵���Ƃ��ɓ����������猩����悤��
		DrawRotaGraph(Game::kScreenCenterX, Game::kScreenCenterY, 1.0, 0.0, m_mainHandle, true);
	}
	else
	{
		if (IsDisappeared())
		{
			return;
		}
		//�V�F�[�_�[
		DissolveUIBase::Draw(Vector2{ Game::kScreenCenterX,Game::kScreenCenterY });
	}
}
