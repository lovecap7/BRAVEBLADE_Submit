#include "SelectStageScene.h"
#include "TitleScene.h"
#include "SceneController.h"
#include "../General/Input.h"
#include "../General/Fader.h"
#include "../General/CSVDataLoader.h"
#include "../General/StageFigure.h"
#include "../General/Sound/SoundManager.h"
#include "../Game/Actors/Character/Player/SelectStagePlayer.h"
#include "../Game/Camera/SelectStageCamera/SelectStageCamera.h"
#include "../Game/UI/Select/SelectStageBackUI.h"
#include "../Game/UI/Select/SelectStageRankingUI.h"
#include "../Game/UI/UIManager.h"
#include "../Game/UI/LeftArrowUI.h"
#include "../SaveData/SaveDataManager.h"
#include "StageScene.h"
#include <memory>
#include <DxLib.h>
#if _DEBUG
//�f�o�b�O���[�h
#include "DebugScene.h"
#endif

namespace
{
	constexpr float kFadeSpeed = 2.0f; // �t�F�[�h���x
	//���UI�̈ʒu
	const Vector2 kLeftArrowPos = Vector2{ 130, 520 };
	const Vector2 kRightArrowPos = Vector2{ 1150, 520 };
}

SelectStageScene::SelectStageScene(SceneController& controller, Stage::StageIndex index):
	SceneBase(controller),
	m_stageIndex(static_cast<int>(index)),
	m_unlockStageIndex(Stage::StageIndex::Stage1),
	m_isDecide(false)
{
	//CSV�f�[�^���[�_�[
	auto csvLodader = std::make_shared<CSVDataLoader>();
	//CSV������W�f�[�^��ǂݍ���
	auto tData = csvLodader->LoadActorDataCSV("Data/CSV/SelectStageTransformData.csv");
	//�X�e�[�W�̈ʒu
	for (auto& data : tData)
	{
		if (data.name == "1")
		{
			m_stagePos[Stage::StageIndex::Stage1] = data.pos;
		}
		else if (data.name == "2")
		{
			m_stagePos[Stage::StageIndex::Stage2] = data.pos;
		}
		else if (data.name == "3")
		{
			m_stagePos[Stage::StageIndex::Stage3] = data.pos;
		}
	}
	//�J����
	m_camera = std::make_unique<SelectStageCamera>(m_stagePos[static_cast<Stage::StageIndex>(m_stageIndex)]);
	//�v���C���[
	m_player = std::make_unique<SelectStagePlayer>(m_camera->GetPos(),m_stagePos[static_cast<Stage::StageIndex>(m_stageIndex)]);
	//�X�e�[�W�̃t�B�M���A
	for (int i = 0; i < m_stageFigures.size(); ++i)
	{
		//�X�e�[�W�̃t�B�M���A���쐬
		m_stageFigures[i] = std::make_shared<StageFigure>();
	}
}

SelectStageScene::~SelectStageScene()
{
}

void SelectStageScene::Init()
{
	//������Ă���X�e�[�W�܂ł����I�ׂȂ����邽�߂̏���
	NowUnlockStage();

	auto& fader = Fader::GetInstance();
	//���񂾂񖾂邭
	fader.FadeIn(kFadeSpeed);
	//�J�����̏�����
	m_camera->Init();
	//�v���C���[�̏�����
	m_player->Init();
	//�X�e�[�W�̃t�B�M���A�̏�����
	m_stageFigures[0]->Init(MV1LoadModel(L"Data/Model/Stage/Select/Stage1Model.mv1"),m_stagePos[Stage::StageIndex::Stage1]);
	m_stageFigures[1]->Init(MV1LoadModel(L"Data/Model/Stage/Select/Stage2Model.mv1"),m_stagePos[Stage::StageIndex::Stage2]);
	m_stageFigures[2]->Init(MV1LoadModel(L"Data/Model/Stage/Select/Stage3Model.mv1"),m_stagePos[Stage::StageIndex::Stage3]);

	//�w�i(�������鏇�Ԃ͕`�扺����)
	auto back1 = std::make_shared<SelectStageBackUI>(LoadGraph(L"Data/UI/Back/Stage1Back.png"));
	auto back2 = std::make_shared<SelectStageBackUI>(LoadGraph(L"Data/UI/Back/Stage2Back.png"));
	auto back3 = std::make_shared<SelectStageBackUI>(LoadGraph(L"Data/UI/Back/Stage3Back.png"));
	back3->Init();
	back2->Init();
	back1->Init();
	m_stage1Back = back1;
	m_stage2Back = back2;
	//�����L���O
	auto rankingUI = std::make_shared<SelectStageRankingUI>(static_cast<Stage::StageIndex>(m_stageIndex));
	rankingUI->Init();
	m_stageRankingUI = rankingUI;
	//���UI
	auto leftArrowUI = std::make_shared<LeftArrowUI>(kLeftArrowPos, false);
	auto rightArrowUI = std::make_shared<LeftArrowUI>(kRightArrowPos, true);
	leftArrowUI->Init();
	rightArrowUI->Init();
	leftArrowUI->SetIsSelect(true);
	rightArrowUI->SetIsSelect(true);
	m_leftArrowUI = leftArrowUI;
	m_rightArrowUI = rightArrowUI;
	//BGM
	SoundManager::GetInstance().PlayBGM("SelectStageBGM");
}

void SelectStageScene::Update()
{
	auto& input = Input::GetInstance();
#if _DEBUG
	//�f�o�b�O�V�[��
	if (input.IsTrigger("SceneChange"))
	{
		//���̃V�[����
		m_controller.ChangeScene(std::make_shared<DebugScene>(m_controller));
		return;
	}
	//�X�e�[�W�S�J��
	if (input.IsTrigger("AllClearStage"))
	{
		auto& saveDatas = SaveDataManager::GetInstance();
		saveDatas.AllClearStage();
		NowUnlockStage();
	}
#endif
	auto& fader = Fader::GetInstance();
	//�^���ÂɂȂ�����
	if (fader.IsFinishFadeOut())
	{
		//���肵�Ă�Ȃ�
		if (m_isDecide)
		{
			//���̃V�[����
			m_controller.ChangeScene(std::make_shared<StageScene>(m_controller, static_cast<Stage::StageIndex>(m_stageIndex)));
			return;
		}
		else
		{
			//�^�C�g����
			m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
			return;
		}
	}
	if (!fader.IsFadeNow())
	{
		//����{�^������������
		if (input.IsTrigger("A"))
		{
			//����SE
			SoundManager::GetInstance().PlayOnceSE("Decide");
			m_isDecide = true;
			//���񂾂�Â�
			fader.FadeOut(kFadeSpeed);
		}
		//�߂�{�^������������
		else if (input.IsTrigger("B"))
		{
			//����SE
			SoundManager::GetInstance().PlayOnceSE("Cancel");
			//���񂾂�Â�
			fader.FadeOut(kFadeSpeed);
		}
	}
	//�X�e�[�W��I��
	SelectStageIndex(input);
	//�X�e�[�W�ɍ��킹�Ĕw�i�𓮂���
	ChangeBack();
	//�J�����̍X�V
	m_camera->Update(m_stagePos[static_cast<Stage::StageIndex>(m_stageIndex)]);
	//�v���C���[�̍X�V
	m_player->Update(m_camera->GetPos(), m_stagePos[static_cast<Stage::StageIndex>(m_stageIndex)]);
	//�X�e�[�W�̃t�B�M���A�̍X�V
	for (auto& stageFigure : m_stageFigures)
	{
		stageFigure->Update();
	}
}

void SelectStageScene::Draw()
{
#if _DEBUG
	DrawString(0, 0, L"SelectStage Scene", 0xffffff);
	DrawString(0, 16, L"[D]�L�[�� Debug Scene", 0xffffff);

	DrawFormatString(0, 80, 0xffff00, L"StageIndex : %d", m_stageIndex);
	DrawFormatString(0, 160, 0xffff00, L"UnlockStageIndex : %d", static_cast<int>(m_unlockStageIndex));
#endif
	//�v���C���[�̕`��
	m_player->Draw();
	//�X�e�[�W�̃t�B�M���A�̕`��
	for (auto& stageFigure : m_stageFigures)
	{
		stageFigure->Draw();
	}	
}

void SelectStageScene::End()
{
	//�v���C���[�̏I��
	m_player->End();
	//�X�e�[�W�̃t�B�M���A�̏I��
	for (auto& stageFigure : m_stageFigures)
	{
		stageFigure->End();
	}
	//����̃f�[�^���Z�[�u
	SaveDataManager::GetInstance().Save();
	//�폜
	UIManager::GetInstance().Reset();

}

void SelectStageScene::Restart()
{
}

void SelectStageScene::SelectStageIndex(Input& input)
{
	auto lastIndex = m_stageIndex;
	//�������Ă�X�e�[�W�܂ł����I�ׂȂ�
	if (input.IsTrigger("Left"))--m_stageIndex;
	if (input.IsTrigger("Right"))++m_stageIndex;
	m_stageIndex = MathSub::ClampInt(m_stageIndex, static_cast<int>(Stage::StageIndex::Stage1), static_cast<int>(m_unlockStageIndex));
	if (m_stageIndex != lastIndex)
	{
		//�Z���N�gSE
		SoundManager::GetInstance().PlayOnceSE("Select");
	}
	if (m_stageRankingUI.expired())return;
	m_stageRankingUI.lock()->SetStageIndex(static_cast<Stage::StageIndex>(m_stageIndex));

	if (m_leftArrowUI.expired() || m_rightArrowUI.expired())return;
	//����N���A���Ă��Ȃ��Ȃ���͉E�������b�N
	if (static_cast<int>(m_unlockStageIndex) == static_cast<int>(Stage::StageIndex::Stage1))
	{
		m_leftArrowUI.lock()->SetIsDraw(false);
		m_rightArrowUI.lock()->SetIsDraw(true);
		m_rightArrowUI.lock()->SetIsLock(true);
		return;
	}
	//�X�e�[�W�̑I���ł�������ɂ���Ė��̕\����ς���
	if(m_stageIndex == static_cast<int>(Stage::StageIndex::Stage1))
	{
		//�E���̂ݕ\��
		m_leftArrowUI.lock()->SetIsDraw(false);
		m_leftArrowUI.lock()->SetIsLock(false);
		m_rightArrowUI.lock()->SetIsDraw(true);
		m_rightArrowUI.lock()->SetIsLock(false);
	}
	//�X�e�[�W3��I�����Ă���Ȃ�
	else if (m_stageIndex == static_cast<int>(Stage::StageIndex::Stage3))
	{
		//�����̂ݕ\��
		m_leftArrowUI.lock()->SetIsDraw(true);
		m_leftArrowUI.lock()->SetIsLock(false);
		m_rightArrowUI.lock()->SetIsDraw(false);
		m_rightArrowUI.lock()->SetIsLock(false);
	}
	//�X�e�[�W2��I�����Ă��āA�X�e�[�W3���������Ă��Ȃ��Ȃ�
	else if (m_stageIndex == static_cast<int>(m_unlockStageIndex))
	{
		//���\���@�E���b�N
		m_leftArrowUI.lock()->SetIsDraw(true);
		m_leftArrowUI.lock()->SetIsLock(false);
		m_rightArrowUI.lock()->SetIsDraw(true);
		m_rightArrowUI.lock()->SetIsLock(true);
	}
	//����ȊO�Ȃ痼���\��
	else
	{
		m_leftArrowUI.lock()->SetIsDraw(true);
		m_leftArrowUI.lock()->SetIsLock(false);
		m_rightArrowUI.lock()->SetIsDraw(true);
		m_rightArrowUI.lock()->SetIsLock(false);
	}
}

void SelectStageScene::ChangeBack()
{
	if (m_stageIndex == static_cast<int>(Stage::StageIndex::Stage1))
	{
		m_stage1Back.lock()->SetAppear(true);
		m_stage2Back.lock()->SetAppear(true);
	}
	else if (m_stageIndex == static_cast<int>(Stage::StageIndex::Stage2))
	{
		m_stage1Back.lock()->SetAppear(false);
		m_stage2Back.lock()->SetAppear(true);
	}
	else
	{
		m_stage1Back.lock()->SetAppear(false);
		m_stage2Back.lock()->SetAppear(false);
	}
}

void SelectStageScene::NowUnlockStage()
{
	auto& saveDatas = SaveDataManager::GetInstance();
	//�X�e�[�W1���N���A���Ă��Ȃ��Ȃ�
	if (!saveDatas.IsClearStage(Stage::StageIndex::Stage1))
	{
		//�X�e�[�W1�����I�ׂȂ�����
		m_unlockStageIndex = Stage::StageIndex::Stage1;
	}
	//�X�e�[�W2���N���A���Ă��Ȃ��Ȃ�
	else if (!saveDatas.IsClearStage(Stage::StageIndex::Stage2))
	{
		//�X�e�[�W2�܂ł����I�ׂȂ�����
		m_unlockStageIndex = Stage::StageIndex::Stage2;
	}
	//�����܂ŗ�����S�ĉ��
	else
	{
		//�S�X�e�[�W���
		m_unlockStageIndex = Stage::StageIndex::Stage3;
	}
}