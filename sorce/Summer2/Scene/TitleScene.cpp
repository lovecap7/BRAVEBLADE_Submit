#include "TitleScene.h"
#include "OptionScene.h"
#include <Dxlib.h>
#include  "../General/Input.h"
#include "SceneController.h"
#include "SelectStageScene.h"
#include "../General/Collision/Physics.h"
#include "../Game/UI/UIManager.h"
#include "../Game/Camera/TitleCamera/TitleCamera.h"
#include "../Game/Actors/Character/Player/TitlePlayer.h"
#include "../Game/Actors/Stage/StageObjectDraw.h"
#include "../Game/Actors/Stage/Sky.h"
#include "../General/CSVDataLoader.h"
#include "../General/Fader.h"
#include "../General/Effect/EffekseerManager.h"
#include "../Game/UI/Title/TitleUI.h"
#include "../Game/UI/MenuUI.h"
#include "../Game/UI/SaveDataUI.h"
#include "../Game/UI/DialogUI.h"
#include "../General/Sound/SoundManager.h"
#include "../General/StringUtil.h"
#include "../Main/Application.h"
#include "../SaveData/SaveDataManager.h"
#include <memory>
#include <cassert>
#if _DEBUG
//�f�o�b�O���[�h
#include "DebugScene.h"
#endif

namespace
{
	//�V���h�E�}�b�v�̕`��T�C�Y
	constexpr int kShadowMapWidth = 1024 * 2;
	constexpr int kShadowMapHeight = 1024 * 2;
	//���C�g�̌���
	const VECTOR kLightDir = { 0.5f, -0.5f, 0.8f };
	//�V���h�E�}�b�v�͈̔�
	constexpr float kShadowMapHorizon = 2000.0f;
	constexpr float kShadowMapVerticalMin = -1.0f;
	constexpr float kShadowMapVerticalMax = 1000.0f;
	//�t�F�[�h�A�E�g
	constexpr float kFadeOutSpeed = 2.0f;
}


TitleScene::TitleScene(SceneController& controller):
	SceneBase(controller),
	m_update(&TitleScene::UpdateTitle),
	m_isDecide(false),
	m_menuIndex(MenuIndex::Continue),
	m_shadowMapHandle(-1),
	m_lightHandle(-1)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	//UI�����Z�b�g
	UIManager::GetInstance().Reset();
	//�J����
	m_camera = std::make_unique<TitleCamera>();
	//�v���C���[
	m_player = std::make_shared<TitlePlayer>();
	//�n���h�����[�h
	LoadHandle();
	//CSV
	auto csvLoader = std::make_shared<CSVDataLoader>();
	//�z�u�f�[�^
	LoadStage(csvLoader);
	//UI
	InitUIs(csvLoader);
	//�^�C�g����ʂ̏�����
	InitTitle();
	//�G�t�F�N�g
	EffekseerManager::GetInstance().CreateTrackActorEffect("FieldEff", m_player);
	//�J�����̏�����
	m_camera->Init();
	//�v���C���[
	m_player->Init();
	//���C�g�̏�����
	InitLight();
	//�e�̏�����
	InitShadow();
	auto& fader = Fader::GetInstance();
	//���񂾂񖾂邭
	fader.FadeIn();
	//BGM�Đ�
	SoundManager::GetInstance().PlayBGM("TitleBGM");
}

void TitleScene::Update()
{
	auto& input = Input::GetInstance();
	if (m_shadowMapHandle >= 0) {
		//�V���h�E�}�b�v���z�肷�郉�C�g�̕������Z�b�g
		SetShadowMapLightDirection(m_shadowMapHandle, kLightDir);
	}
	//��ԍX�V
	(this->*m_update)(input);
}

void TitleScene::Draw()
{
#if _DEBUG
	DrawString(0, 0, L"Title Scene", 0xffffff);
	DrawString(0, 16, L"[D]�L�[�� Debug Scene", 0xffffff);
#endif
	//�V���h�E�}�b�v�ւ̕`��̏���
	ShadowMap_DrawSetup(m_shadowMapHandle);
	//�v���C���[�̕`��
	m_player->Draw();
	//�X�e�[�W�`��
	for (auto& obj : m_stageObjects)
	{
		obj->Draw();
	}
	//�V���h�E�}�b�v�ւ̕`����I��
	ShadowMap_DrawEnd();
	//�`��Ɏg�p����V���h�E�}�b�v��ݒ�
	SetUseShadowMap(0, m_shadowMapHandle);
	//�v���C���[�̕`��
	m_player->Draw();
	//�X�e�[�W�`��
	for (auto& obj : m_stageObjects)
	{
		obj->Draw();
	}
	//�`��Ɏg�p����V���h�E�}�b�v�̐ݒ������
	SetUseShadowMap(0, -1);
}

void TitleScene::End()
{
	//�v���C���[�̏I��
	m_player->End();
	//�n���h���폜
	AllDeleteHandle();
	//�I�u�W�F�폜
	AllDeleteStage();
	//���C�g�̍폜
	DeleteLightHandle(m_lightHandle);
	//�V���h�E�}�b�v�̍폜
	DeleteShadowMap(m_shadowMapHandle);
	//UI
	UIManager::GetInstance().Reset();
	//�G�t�F�N�g
	EffekseerManager::GetInstance().Reset();
}

void TitleScene::LoadHandle()
{
	m_stageHandles["Block_Grass"] = { MV1LoadModel(L"Data/Model/Stage/1/Block_Grass.mv1") };
	m_stageHandles["Grass1"] = { MV1LoadModel(L"Data/Model/Stage/Title/Grass1.mv1") };
	m_stageHandles["Rock1"] = { MV1LoadModel(L"Data/Model/Stage/Title/Rock1.mv1") };
	m_stageHandles["Rock2"] = { MV1LoadModel(L"Data/Model/Stage/Title/Rock2.mv1") };
	m_stageHandles["Tree1"] = { MV1LoadModel(L"Data/Model/Stage/Title/Tree1.mv1") };
	m_stageHandles["Flower1"] = { MV1LoadModel(L"Data/Model/Stage/Title/Flower1.mv1") };
	m_stageHandles["Sky"] = { MV1LoadModel(L"Data/Model/Stage/Sky/Sky_Daylight02.pmx") };
	//���[�h�ɐ����������`�F�b�N
	for (auto& [key, value] : m_stageHandles) {
		assert(value >= 0);
	}
}
void TitleScene::LoadStage(std::shared_ptr<CSVDataLoader> csvLoader)
{
	//����쐬
	m_stageObjects.emplace_back(std::make_shared<Sky>(MV1DuplicateModel(m_stageHandles["Sky"])));
	auto datas = csvLoader->LoadActorDataCSV("Data/CSV/TitleTransformData.csv");
	//���O����I�u�W�F�N�g��z�u���Ă���
	for (auto& data : datas)
	{
		if (data.name == "Block_Grass")
		{
			//�傫����1/100���Ȃ��Ƒ傫������̂�
			data.scale = VScale(data.scale, 0.01f);
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Block_Grass")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Grass1")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Grass1")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Rock1")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Rock1")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Rock2")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Rock2")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Tree1")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Tree1")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
		else if (data.name == "Flower1")
		{
			std::shared_ptr<StageObjectDraw> blockGrass =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_stageHandles.at("Flower1")), data.pos, data.scale, data.rot);
			m_stageObjects.emplace_back(blockGrass);
		}
	}
}

void TitleScene::AllDeleteHandle()
{
	for (auto& [key, value] : m_stageHandles) {
		if (value >= 0)
		{
			auto result = MV1DeleteModel(value);
			assert(result == 0);
		}
	}
	m_stageHandles.clear();
}

void TitleScene::AllDeleteStage()
{
	//�X�e�[�W�`��
	for (auto& obj : m_stageObjects)
	{
		obj->End();
	}
	m_stageObjects.clear();
	m_player.reset();
	m_camera.reset();
}

void TitleScene::UpdateTitle(Input& input)
{
#if _DEBUG
	//�f�o�b�O�V�[��
	if (input.IsTrigger("SceneChange"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<DebugScene>(m_controller));
		return;
	}
#endif
	//�����{�^������������
	if (input.IsTriggerAny())
	{
		//�o���������Ă�Ȃ�
		if (m_titleUI.lock()->IsAppered())
		{
			//����SE
			SoundManager::GetInstance().PlayOnceSE("Decide");
			InitSelectMenu();
			return;
		}
		else
		{
			//���S�ɏo��������
			m_titleUI.lock()->DissolveRateMin();
		}
	}
	//���ʂ̍X�V����
	UpdateCommon();
	
}
void TitleScene::UpdateSelectMenu(Input& input)
{
	auto& fader = Fader::GetInstance();
	//�߂�{�^������������
	if (input.IsTrigger("B") && m_titleUI.lock()->IsAppered() && !fader.IsFadeNow())
	{
		//�L�����Z��SE
		SoundManager::GetInstance().PlayOnceSE("Cancel");
		//����������
		InitTitle();
		return;
	}
	//���ʂ̍X�V����
	UpdateCommon();
	//�㉺�ɃX�e�b�N�𓮂����ă��j���[�̍��ڂ�I������
	SelectMenu(input);

}

void TitleScene::UpdateDialog(Input& input)
{
	auto& fader = Fader::GetInstance();
	//�^���ÂɂȂ�����
	if (fader.IsFinishFadeOut())
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<SelectStageScene>(m_controller,Stage::StageIndex::Stage1));
		return;
	}
	if (!fader.IsFadeNow())
	{
		if (input.IsTrigger("Left"))m_dialogUI.lock()->SelectYes();
		if (input.IsTrigger("Right"))m_dialogUI.lock()->SelectNo();
		//���肵�����C���f�b�N�X���珈���𕪊�
		if (input.IsTrigger("A"))
		{
			//"�͂�"��I�񂾏ꍇ
			if (m_dialogUI.lock()->IsYes())
			{
				//����SE
				SoundManager::GetInstance().PlayOnceSE("Decide");
				switch (m_menuIndex)
				{
				case MenuIndex::Continue:
					Continue();
					break;
				case MenuIndex::NewGame:
					NewGame();
					break;
				case MenuIndex::FinishGame:
					FinishGame();
					break;
				}
				return;
			}
			//"������"��I�񂾏ꍇ
			else
			{
				InitSelectMenu();
				//�L�����Z��SE
				SoundManager::GetInstance().PlayOnceSE("Cancel");
				m_update = &TitleScene::UpdateSelectMenu;
				return;
			}
		}
		if (input.IsTrigger("B"))
		{
			InitSelectMenu();
			//�L�����Z��SE
			SoundManager::GetInstance().PlayOnceSE("Cancel");
			m_update = &TitleScene::UpdateSelectMenu;
			return;
		}
	}
	//���ʂ̍X�V����
	UpdateCommon();
}

void TitleScene::SelectMenu(Input& input)
{
	//�I��
	auto menuIndex = static_cast<int>(m_menuIndex);
	if (input.IsRepeate("Up"))	--menuIndex;
	if (input.IsRepeate("Down"))++menuIndex;
	//�l���ω�������
	if (menuIndex != static_cast<int>(m_menuIndex))
	{
		//�Z���N�gSE
		SoundManager::GetInstance().PlayOnceSE("Select");
	}
	menuIndex = MathSub::ClampInt(menuIndex, static_cast<int>(MenuIndex::Continue), static_cast<int>(MenuIndex::FinishGame));
	m_menuIndex = static_cast<MenuIndex>(menuIndex);
	//�I�΂�Ă��鍀�ڂɑΉ�����UI�ɑI�΂�Ă��邱�Ƃ��t���O�œ`����
	for (auto& menuUI : m_menuUIs)
	{
		//��x���Z�b�g
		menuUI.second.lock()->SetIsSelect(false);
	}
	//�I�΂�Ă��鍀�ڂ̂�true
	if (m_menuUIs[m_menuIndex].expired())return;
	m_menuUIs[m_menuIndex].lock()->SetIsSelect(true);
	auto& fader = Fader::GetInstance();
	//���肵�����C���f�b�N�X���珈���𕪊�
	if (input.IsTrigger("A") && m_titleUI.lock()->IsAppered() && !fader.IsFadeNow())
	{
		//����SE
		SoundManager::GetInstance().PlayOnceSE("Decide");
		switch (m_menuIndex)
		{
		case MenuIndex::Continue:
			InitContinue();
			break;
		case MenuIndex::NewGame:
			InitNewGame();
			break;
		case MenuIndex::Option:
			Option();
			return;
			break;
		case MenuIndex::FinishGame:
			InitFinishGame();
			break;
		}
		m_update = &TitleScene::UpdateDialog;
		return;
	}
}

void TitleScene::InitContinue()
{
	//�g�債����Ԃőҋ@
	if (m_menuUIs[m_menuIndex].expired())return;
	m_menuUIs[m_menuIndex].lock()->SetIsWait(true);
	//�_�C�A���O���o��
	if (m_dialogUI.expired())return;
	m_dialogUI.lock()->SetIsDraw(true);
	m_dialogUI.lock()->SetText(L"��������n�߂܂���?");
	m_dialogUI.lock()->SelectYes();
}

void TitleScene::Continue()
{
	auto& fader = Fader::GetInstance();
	//���񂾂�Â�
	fader.FadeOut(kFadeOutSpeed);
	m_isDecide = true;
}

void TitleScene::InitNewGame()
{
	//�g�債����Ԃőҋ@
	if (m_menuUIs[m_menuIndex].expired())return;
	m_menuUIs[m_menuIndex].lock()->SetIsWait(true);
	//�_�C�A���O���o��
	if (m_dialogUI.expired())return;
	m_dialogUI.lock()->SetIsDraw(true);
	m_dialogUI.lock()->SetText(L"�f�[�^���폜����\n�ŏ�����n�߂܂���?");
	m_dialogUI.lock()->SelectYes();
}

void TitleScene::NewGame()
{
	auto& fader = Fader::GetInstance();
	//���񂾂�Â�
	fader.FadeOut(kFadeOutSpeed);
	m_isDecide = true;
	//�f�[�^���폜
	SaveDataManager::GetInstance().NewGame();
}

void TitleScene::Option()
{
	m_controller.PushScene(std::make_shared<OptionScene>(m_controller));
	return;
}

void TitleScene::InitFinishGame()
{
	//�g�債����Ԃőҋ@
	if (m_menuUIs[m_menuIndex].expired())return;
	m_menuUIs[m_menuIndex].lock()->SetIsWait(true);
	//�_�C�A���O���o��
	if (m_dialogUI.expired())return;
	m_dialogUI.lock()->SetIsDraw(true);
	m_dialogUI.lock()->SetText(L"�Q�[�����I�����܂���?");
	m_dialogUI.lock()->SelectYes();
}

void TitleScene::FinishGame()
{
	//�A�v���P�[�V�����̏I��
	Application::GetInstance().FinishApplication();
}

void TitleScene::InitLight()
{
	m_lightHandle = CreateDirLightHandle(kLightDir);
}

void TitleScene::InitShadow()
{
	//�V���h�E�}�b�v�n���h���̍쐬
	m_shadowMapHandle = MakeShadowMap(kShadowMapWidth, kShadowMapHeight);
}

void TitleScene::UpdateShadow()
{
	auto shadowMinPos = Vector3::Zero();
	shadowMinPos.x = -kShadowMapHorizon;
	shadowMinPos.y = kShadowMapVerticalMin;
	shadowMinPos.z = -kShadowMapHorizon;
	auto shadowMaxPos = Vector3::Zero();
	shadowMaxPos.x = kShadowMapHorizon;
	shadowMaxPos.y = kShadowMapVerticalMax;
	shadowMaxPos.z = kShadowMapHorizon;
	SetShadowMapDrawArea(m_shadowMapHandle, shadowMinPos.ToDxLibVector(), shadowMaxPos.ToDxLibVector());
}

void TitleScene::UpdateCommon()
{
	//�J�����X�V
	m_camera->Update();
	//�v���C���[�X�V
	m_player->Update(m_isDecide);
	//�X�e�[�W�X�V
	for (auto& obj : m_stageObjects)
	{
		obj->Update();
	}
	//�e
	UpdateShadow();
}

void TitleScene::InitUIs(std::shared_ptr<CSVDataLoader>& csvLoader)
{
	auto& uiManager = UIManager::GetInstance();
	auto titleUI = std::make_shared<TitleUI>();
	titleUI->Init();
	m_titleUI = titleUI;
	//���j���[UI
	auto menuData = csvLoader->LoadUIDataCSV("Data/CSV/TitleMenuUITransformData.csv");
	for (int i = 0;i < menuData.size();++i)
	{
		int handle = -1;
		switch (static_cast<MenuIndex>(i))
		{
		case MenuIndex::Continue:
			handle = uiManager.GetImageHandle("ContinueT");
			break;
		case MenuIndex::NewGame:
			handle = uiManager.GetImageHandle("NewGame");
			break;
		case MenuIndex::Option:
			handle = uiManager.GetImageHandle("Option");
			break;
		case MenuIndex::FinishGame:
			handle = uiManager.GetImageHandle("FinishGame");
			break;
		default:
			break;
		}
		auto menuUI = std::make_shared<MenuUI>(menuData[i].pos, handle);
		menuUI->Init();
		m_menuUIs[static_cast<MenuIndex>(i)] = menuUI;
	}
	//�Z�[�u�f�[�^
	auto saveDataUI = std::make_shared<SaveDataUI>();
	saveDataUI->Init();
	m_saveDataUI = saveDataUI;
	//�_�C�A���O
	auto dialogUI = std::make_shared<DialogUI>();
	dialogUI->Init();
	m_dialogUI = dialogUI;
}

void TitleScene::InitTitle()
{
	//���j���[UI�̔�\��
	for (auto& menuUI : m_menuUIs)
	{
		menuUI.second.lock()->SetIsDraw(false);
	}
	//�^�C�g����\���ƃ��Z�b�g
	m_titleUI.lock()->SetIsDraw(true);
	m_titleUI.lock()->Reset();
	//�Z�[�u�f�[�^���\��
	m_saveDataUI.lock()->SetIsDraw(false);
	//�_�C�A���O���\��
	m_dialogUI.lock()->SetIsDraw(false);
	//���̏�Ԃ�
	m_update = &TitleScene::UpdateTitle;
}

void TitleScene::InitSelectMenu()
{
	//���j���[UI�̕\���ƃ��Z�b�g
	for (auto& menuUI : m_menuUIs)
	{
		menuUI.second.lock()->SetIsDraw(true);
	}
	//�ҋ@����
	if (m_menuUIs[m_menuIndex].expired())return;
	m_menuUIs[m_menuIndex].lock()->SetIsWait(false);
	//�^�C�g�����\��
	if (m_titleUI.expired())return;
	m_titleUI.lock()->SetIsDraw(false);
	//�Z�[�u�f�[�^��\��
	if (m_saveDataUI.expired())return;
	m_saveDataUI.lock()->SetIsDraw(true);
	//�_�C�A���O���\��
	if (m_dialogUI.expired())return;
	m_dialogUI.lock()->SetIsDraw(false);
	//���̏�Ԃ�
	m_update = &TitleScene::UpdateSelectMenu;
}
