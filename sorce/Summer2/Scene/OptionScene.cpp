#include "OptionScene.h"
#include "SceneController.h"
#include "../Main/Application.h"
#include <DxLib.h>
#include "../General/game.h"
#include "../General/Input.h"
#include "../General/Sound/SoundManager.h"
#include "../Game/UI/UIManager.h"
#include "../Game/UI/Option/ScreenModeUI.h"
#include "../Game/UI/Option/VolumeUI.h"
#include "../Game/UI/LeftArrowUI.h"

namespace
{
	//���ʒ����̃��s�[�g�t���[��
	constexpr int kRepeateFrame = 2;
	//���WY
	constexpr int kScreenModeY	= 130;
	constexpr int kMasterPosY	= 260;
	constexpr int kBGMPosY		= 370;
	constexpr int kSEPosY		= 480;
	constexpr int kVoicePosY	= 590;
	//���̍��W
	constexpr int kArrowPosX = 330;
}

OptionScene::OptionScene(SceneController& controller):
	SceneBase(controller),
	m_optionIndex(OptionIndex::ScreenMode)
{
}

OptionScene::~OptionScene()
{
}

void OptionScene::Init()
{
	//�X�N���[�����[�hUI
	auto screenModeUI = std::make_shared<ScreenModeUI>(Vector2{ Game::kScreenCenterX,kScreenModeY });
	screenModeUI->Init();
	m_screenModeUI = screenModeUI;
	//�}�X�^�[
	auto masterUI = std::make_shared<VolumeUI>(kMasterPosY,L"Master");
	masterUI->Init();
	m_masterUI = masterUI;
	//BGM
	auto bgmUI = std::make_shared<VolumeUI>(kBGMPosY,L"BGM");
	bgmUI->Init();
	m_bgmUI = bgmUI;
	//SE
	auto seUI = std::make_shared<VolumeUI>(kSEPosY,L"SE");
	seUI->Init();
	m_seUI = seUI;
	//Voice
	auto voiceUI = std::make_shared<VolumeUI>(kVoicePosY,L"Voice");
	voiceUI->Init();
	m_voiceUI = voiceUI;
	//���ʃZ�b�g
	auto& soundManager = SoundManager::GetInstance();
	m_masterUI.lock()->SetVolume(soundManager.GetMasterVolume());
	m_bgmUI.lock()->SetVolume(soundManager.GetBGMVolume());
	m_seUI.lock()->SetVolume(soundManager.GetSEVolume());
	m_voiceUI.lock()->SetVolume(soundManager.GetVoiceVolume());
	//�I�v�V�����I����
	auto optionArrowUI = std::make_shared<LeftArrowUI>(Vector2{ kArrowPosX, 0 }, true);
	optionArrowUI->Init();
	optionArrowUI->SetIsSelect(true);
	optionArrowUI->SetBaseScale(0.5);
	m_optionArrowUI = optionArrowUI;
}

void OptionScene::Update()
{
	auto& input = Input::GetInstance();
	//���ڑI��
	int optionIndex = static_cast<int>(m_optionIndex);
	if (input.IsTrigger("Up"))--optionIndex;
	if (input.IsTrigger("Down"))++optionIndex;
	optionIndex = MathSub::ClampInt(optionIndex, static_cast<int>(OptionIndex::ScreenMode), static_cast<int>(OptionIndex::VoiceVolume));
	//���ڂ��ς������
	if (m_optionIndex != static_cast<OptionIndex>(optionIndex))
	{
		//�Z���N�gSE
		SoundManager::GetInstance().PlayOnceSE("Select");
	}
	m_optionIndex = static_cast<OptionIndex>(optionIndex);
	if (input.IsTrigger("B"))
	{
		//�L�����Z��SE
		SoundManager::GetInstance().PlayOnceSE("Cancel");
		m_controller.PopScene();
		return;
	}
	//�I�΂�Ă鍀�ڂɑΉ���������
	//���UI�̈ʒu�ύX
	if (!m_optionArrowUI.expired())
	{
		Vector2 arrowPos = {};
		switch (m_optionIndex)
		{
		case OptionScene::OptionIndex::ScreenMode:
			arrowPos = { kArrowPosX, kScreenModeY };
			break;
		case OptionScene::OptionIndex::MasterVolume:
			arrowPos = { kArrowPosX, kMasterPosY };
			break;
		case OptionScene::OptionIndex::BGMVolume:
			arrowPos = { kArrowPosX, kBGMPosY };
			break;
		case OptionScene::OptionIndex::SEVolume:
			arrowPos = { kArrowPosX, kSEPosY };
			break;
		case OptionScene::OptionIndex::VoiceVolume:
			arrowPos = { kArrowPosX, kVoicePosY };
			break;
		default:
			break;
		}
		m_optionArrowUI.lock()->SetPos(arrowPos);
	}
	switch (m_optionIndex)
	{
	case OptionScene::OptionIndex::ScreenMode:
		OptionSreenMode(input);
		break;
	case OptionScene::OptionIndex::MasterVolume:
		OptionMasterVolume(input);
		break;
	case OptionScene::OptionIndex::BGMVolume:
		OptionBGMVolume(input);
		break;
	case OptionScene::OptionIndex::SEVolume:
		OptionSEVolume(input);
		break;
	case OptionScene::OptionIndex::VoiceVolume:
		OptionVoiceVolume(input);
		break;
	default:
		break;
	}
}

void OptionScene::Draw()
{
	
}

void OptionScene::End()
{
	if(!m_screenModeUI.expired()) m_screenModeUI.lock()->Delete();
	if(!m_masterUI.expired()) m_masterUI.lock()->Delete();
	if(!m_bgmUI.expired()) m_bgmUI.lock()->Delete();
	if(!m_seUI.expired()) m_seUI.lock()->Delete();
	if(!m_voiceUI.expired()) m_voiceUI.lock()->Delete();
	if(!m_optionArrowUI.expired()) m_optionArrowUI.lock()->Delete();
	SoundManager::GetInstance().SaveVolume();
}

void OptionScene::Restart()
{
}

void OptionScene::OptionSreenMode(Input& input)
{
	if (input.IsTrigger("Right") || input.IsTrigger("Left"))
	{
		//�؂�ւ�
		auto& application = Application::GetInstance();
		application.SetWindowMode(!application.IsWindowMode());
	}
}

void OptionScene::OptionMasterVolume(Input& input)
{
	auto volume = SoundManager::GetInstance().GetMasterVolume();
	if (input.IsRepeate("Left", kRepeateFrame))--volume;
	if (input.IsRepeate("Right", kRepeateFrame))++volume;
	//UI�ɔ��f
	m_masterUI.lock()->SetVolume(volume);
	SoundManager::GetInstance().SetMasterVolume(volume);
}
void OptionScene::OptionBGMVolume(Input& input)
{
	auto volume = SoundManager::GetInstance().GetBGMVolume();
	if (input.IsRepeate("Left", kRepeateFrame))--volume;
	if (input.IsRepeate("Right", kRepeateFrame))++volume;
	//UI�ɔ��f
	m_bgmUI.lock()->SetVolume(volume);
	SoundManager::GetInstance().SetBGMVolume(volume);
}
void OptionScene::OptionSEVolume(Input& input)
{
	auto volume = SoundManager::GetInstance().GetSEVolume();
	if (input.IsRepeate("Left", kRepeateFrame))--volume;
	if (input.IsRepeate("Right", kRepeateFrame))++volume;
	//UI�ɔ��f
	m_seUI.lock()->SetVolume(volume);
	SoundManager::GetInstance().SetSEVolume(volume);
}
void OptionScene::OptionVoiceVolume(Input& input)
{
	auto volume = SoundManager::GetInstance().GetVoiceVolume();
	if (input.IsRepeate("Left", kRepeateFrame))--volume;
	if (input.IsRepeate("Right", kRepeateFrame))++volume;
	//UI�ɔ��f
	m_voiceUI.lock()->SetVolume(volume);
	SoundManager::GetInstance().SetVoiceVolume(volume);
}