#include "TutorialUI.h"
#include "UIManager.h"
#include "../../General/game.h"
#include "../../General/StringUtil.h"
#include "../../General/Math/MyMath.h"
#include <DxLib.h>

namespace
{
	//�l�p�̍��W
	constexpr int kSpaceLeftPos = Game::kScreenCenterX - 100;
	constexpr int kSpaceUpPos = 60;
	constexpr int kSpaceRightPos = Game::kScreenWidth;
	constexpr int kSpaceDownPos = 135;
	//���s�̐������l�p�̉��̍��W�������Ă�
	constexpr int kSpaceDownSize = 20;
	//�h�`���[�g���A���h�̕\���ʒu
	constexpr int kTutorialOffsetPosX = 50;
	constexpr int kTutorialPosY = kSpaceUpPos + 10;
	//�P�s�ɓ��镶����̐�
	constexpr int kOneLineMaxNum = 30;
	//�e�L�X�g�̍��W
	constexpr int kTextOffsetPosX = 50;
	constexpr int kTextPosY = kTutorialPosY + 30;
	//�e�L�X�g�̕\�����x
	constexpr int kTextViewSpeed = 2;
	//��������
	constexpr int kAppearInterval = 20;
	constexpr int kNormalInterval = 10 * 60;
	//�����x
	constexpr float kAlphaRate = 200;
	//�g���̑���
	constexpr int kFrameThickness = 10;
	//�`���[�g���A���}�[�N�̍��W
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
	m_textHandle(UIManager::GetInstance().GetTextHandle("�����S�V�b�N20")),
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
	//�`���[�g���A���̃X�y�[�X
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlphaRate);
	//���s�̐�
	auto lineNum = StringUtil::WStringLineNum(m_text);
	DrawBox(m_posX - kFrameThickness, kSpaceUpPos - kFrameThickness, kSpaceRightPos, kSpaceDownPos + kSpaceDownSize * lineNum + kFrameThickness, 0xffffff, true);
	DrawBox(m_posX, kSpaceUpPos, kSpaceRightPos, kSpaceDownPos + kSpaceDownSize * lineNum, 0x444444, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//�e�L�X�g
	DrawStringToHandle(m_posX + kTutorialOffsetPosX, kTutorialPosY, L"<�`���[�g���A��>", 0xffffff, m_textHandle);
	DrawStringToHandle(m_posX + kTextOffsetPosX, kTextPosY, m_text.substr(0, m_chatCount).c_str(), 0xffffff, m_textHandle);
	//�`���[�g���A���}�[�N
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
	//�\���t���[��
	if (m_countFrame > kNormalInterval)
	{
		m_countFrame = kAppearInterval;
		m_update = &TutorialUI::DisappearUpdate;
		return;
	}
	//�e�L�X�g��i�߂�
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
