#include "UIManager.h"
#include "UIBase.h"
#include "../Actors/ActorManager.h"
#include <cassert>
//�v���C���[
#include "PlayerUI/PlayerHPUI.h"
#include "PlayerUI/PlayerUltGageUI.h"
//�{�X
#include "EnemyUI/BossHPUI.h"
//�G
#include "EnemyUI/EnemyHPUI.h"
//�X�R�A
#include "GameScoreUI.h"
//�^�C�}�[
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
	//���łɓo�^����Ă���Ȃ炵�Ȃ�
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it != m_uis.end())return;
	//�o�^
	m_uis.emplace_back(ui);
}

void UIManager::Exit(std::shared_ptr<UIBase> ui)
{
	//�o�^����Ă��Ȃ��Ȃ炵�Ȃ�
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it == m_uis.end())return;
	m_uis.erase(it);
}
void UIManager::Init()
{
	//�n���h�����[�h
	LoadHandle();
	//�X�V�ƕ`��t���O
	m_isUpdate = true;
	m_isDraw = true;
}

void UIManager::Update()
{
	//�X�V�����邩
	if (!m_isUpdate)return;
	//�폜�\���UI�폜
	CheckDelete();
	//�X�V
	for (auto& ui : m_uis)
	{
		ui->Update();
	}
}

void UIManager::BackDraw() const
{
	//�`������邩
	if (!m_isDraw)return;
	//�D��x�Ⴂ���ɕ`��
	for (auto& ui : m_uis)
	{
		//���`�悶��Ȃ��Ȃ�
		if (ui->IsFront())continue;
		if (ui->GetPriority() != UIBase::Priority::Low)continue;
		ui->Draw();
	}
	for (auto& ui : m_uis)
	{
		//���`�悶��Ȃ��Ȃ�
		if (ui->IsFront())continue;
		if (ui->GetPriority() != UIBase::Priority::Middle)continue;
		ui->Draw();
	}
	for (auto& ui : m_uis)
	{
		//���`�悶��Ȃ��Ȃ�
		if (ui->IsFront())continue;
		if (ui->GetPriority() != UIBase::Priority::High)continue;
		ui->Draw();
	}
	
}

void UIManager::FrontDraw() const
{
	//�`������邩
	if (!m_isDraw)return;
	//�D��x�Ⴂ���ɕ`��
	for (auto& ui : m_uis)
	{
		//�O�`�悶��Ȃ��Ȃ�
		if (!ui->IsFront())continue;
		if (ui->GetPriority() != UIBase::Priority::Low)continue;
		ui->Draw();
	}
	for (auto& ui : m_uis)
	{
		//�O�`�悶��Ȃ��Ȃ�
		if (!ui->IsFront())continue;
		if (ui->GetPriority() != UIBase::Priority::Middle)continue;
		ui->Draw();
	}
	for (auto& ui : m_uis)
	{
		//�O�`�悶��Ȃ��Ȃ�
		if (!ui->IsFront())continue;
		if (ui->GetPriority() != UIBase::Priority::High)continue;
		ui->Draw();
	}
}

void UIManager::End()
{
	//���ׂč폜
	AllDeleteUIs();
	//�n���h���폜
	AllDeleteHandle();
}

void UIManager::Reset()
{
	//UI�݂̂��ׂč폜
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
		//���O���o�����f�t�H���g�l��Ԃ�
		assert(false && "ImageHandles not found");
		return -1;
	}
	return it->second;
}

int UIManager::GetTextHandle(const std::string& name) const
{
	auto it = m_textHandles.find(name);
	if (it == m_textHandles.end()) {
		//���O���o�����f�t�H���g�l��Ԃ�
		assert(false && "TextHandle not found");
		return -1;
	}
	return it->second;
}

void UIManager::LoadHandle()
{
	//�t�H���g�̓ǂݍ���
	LPCSTR fontPath = "Font/�����S�V�b�N EMG 3 - BOLD.ttf";
	if (AddFontResourceExA(fontPath, FR_PRIVATE, NULL) > 0)
	{
	}
	else
	{
		// �t�H���g�Ǎ��G���[����
		assert(0 && "�t�H���g�ǂݍ��ݎ��s");
	}
	//�摜�n���h��
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
	//���j���[
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
	//���[�h�ɐ����������`�F�b�N
	for (auto& [key, value] : m_imageHandles)
	{
		assert(value >= 0);
	}
	//�e�L�X�g�n���h��
	m_textHandles["���C���I16"] = { CreateFontToHandle(L"Meiryo", kFontSize16, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["���C���I32"] = { CreateFontToHandle(L"Meiryo", kFontSize32, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["���C���I48"] = { CreateFontToHandle(L"Meiryo", kFontSize48, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["���C���I64"] = { CreateFontToHandle(L"Meiryo", kFontSize64, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["���C���I80"] = { CreateFontToHandle(L"Meiryo", kFontSize80, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["���C���I96"] = { CreateFontToHandle(L"Meiryo", kFontSize96, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["MSP�S�V�b�N16"] = { CreateFontToHandle(L"MS PGothic", kFontSize16, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["MSP�S�V�b�N32"] = { CreateFontToHandle(L"MS PGothic", kFontSize32, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["MSP�S�V�b�N48"] = { CreateFontToHandle(L"MS PGothic", kFontSize48, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["MSP�S�V�b�N64"] = { CreateFontToHandle(L"MS PGothic", kFontSize64, 5, DX_FONTTYPE_ANTIALIASING) };
	m_textHandles["�����S�V�b�N16"] = { CreateFontToHandle(L"�����S�V�b�N EMG 3 - BOLD", kFontSize16, 5, DX_FONTTYPE_ANTIALIASING_EDGE) };
	m_textHandles["�����S�V�b�N20"] = { CreateFontToHandle(L"�����S�V�b�N EMG 3 - BOLD", kFontSize20, 5, DX_FONTTYPE_ANTIALIASING_EDGE) };
	m_textHandles["�����S�V�b�N32"] = { CreateFontToHandle(L"�����S�V�b�N EMG 3 - BOLD", kFontSize32, 5, DX_FONTTYPE_ANTIALIASING_EDGE) };
	m_textHandles["�����S�V�b�N48"] = { CreateFontToHandle(L"�����S�V�b�N EMG 3 - BOLD", kFontSize48, 5, DX_FONTTYPE_ANTIALIASING_EDGE) };
	//���[�h�ɐ����������`�F�b�N
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
	//�摜�̍폜
	for (auto& [key, value] : m_imageHandles) {
		if (value >= 0)
		{
			auto result = DeleteGraph(value);
			assert(result == 0);
		}
	}
	m_imageHandles.clear();
	//�e�L�X�g�̍폜
	for (auto& [key, value] : m_textHandles) {
		DeleteFontToHandle(value);
	}
	m_textHandles.clear();
	// �E�B���h�E�Y�Ɉꎞ�I�ɕێ����Ă����t�H���g�f�[�^���폜
	RemoveFontResourceExA("", FR_PRIVATE, NULL);
}

void UIManager::CheckDelete()
{
	auto remIt = std::remove_if(m_uis.begin(), m_uis.end(), [](std::shared_ptr<UIBase> ui) {
		return  ui->IsDelete();
		});
	m_uis.erase(remIt, m_uis.end());//�폜
}
