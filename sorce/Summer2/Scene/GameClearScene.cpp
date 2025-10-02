#include "GameClearScene.h"
#include "SelectStageScene.h"
#include "../General/Input.h"
#include "SceneController.h"
#include<DxLib.h>
#include "../General/game.h"
#include "../General/Collision/Physics.h"
#include "../General/Sound/SoundManager.h"
#include "../Game/GameRule/Score.h"
#include "../General/Fader.h"
#include "../General/Sound/SE.h"
#include "../General/Effect/EffekseerManager.h"
#include "../Game/UI/UIManager.h"
#include "../Game/UI/Result/ResultUI.h"
#include "../Game/UI/Result/ResultScoreUI.h"
#include "../Game/UI/Result/ResultRankingScore.h"
#include "../SaveData/SaveDataManager.h"

namespace {
	constexpr int kAppearInterval = 60;
	constexpr int kAlphaRate = 2;
	constexpr int kFrameMargin = 10;//�Q�[����ʂ���|�[�Y��ʂ܂ł̕�
}

GameClearScene::GameClearScene(SceneController& controller, Stage::StageIndex index):
	SceneBase(controller),
	m_stageIndex(index),
	m_update(&GameClearScene::AppearUpdate),
	m_draw(&GameClearScene::NormalDraw),
	m_countFrame(0)
{
	//����BGM
	SoundManager::GetInstance().PlayBGM("ResultBGM");
	//���ZSE
	m_addScoreSE = SoundManager::GetInstance().PlayLoopSE("AddScore");
	//�~�߂�
	m_addScoreSE.lock()->Stop();
}

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Init()
{
	auto& uiManager = UIManager::GetInstance();
	//UI�\��
	uiManager.StartDraw();
	uiManager.StartUpdate();
	//��xUI�����Z�b�g
	uiManager.Reset();
	//Physics���~�߂�
	Physics::GetInstance().StopUpdate();
	//�G�t�F�N�g��~
	EffekseerManager::GetInstance().StopEffect();
	//�N���A�������Ƃ��L�^����
	auto& saveDataManager = SaveDataManager::GetInstance();
	saveDataManager.SaveClearStage(m_stageIndex);
	//UI
	std::make_shared<ResultUI>()->Init();
}

void GameClearScene::Update()
{
	(this->*m_update)();
}

void GameClearScene::Draw()
{
	(this->*m_draw)();
}
void GameClearScene::End()
{
	//Physics���J�n
	Physics::GetInstance().StartUpdate();
	//�G�t�F�N�g�J�n
	EffekseerManager::GetInstance().StartEffect();
	//SE�폜
	if(!m_addScoreSE.expired())m_addScoreSE.lock()->Delete();
}

void GameClearScene::Restart()
{
}

void GameClearScene::AppearUpdate()
{
	++m_countFrame;
	if (m_countFrame > kAppearInterval)
	{
		//�~�߂�
		m_addScoreSE.lock()->Play();
		//���U���gUI
		InitResult1UI();
		m_countFrame = kAppearInterval;
		m_update = &GameClearScene::Result1Update;
		return;
	}
}

void GameClearScene::Result1Update()
{
	auto& input = Input::GetInstance();
	bool isAllMax = true;
	for (auto& scoreUI : m_scoreUIList)
	{
		if (!scoreUI.lock()->IsViewScoreMax())
		{
			isAllMax = false;
			break;
		}
	}
	//�S�ẴX�R�AUI���ő�l�ɂȂ�����
	if (isAllMax)
	{
		//�~�߂�
		m_addScoreSE.lock()->Stop();
		//A�{�^���Ŏ���
		if (input.IsTrigger("A"))
		{
			//�~�߂�
			m_addScoreSE.lock()->Play();
			//�����܂ŗ����玟�̏�Ԃ�
			//�����L���OUI
			InitResult2UI();
			//���̏��
			m_update = &GameClearScene::Result2Update;
			return;
		}
	}
}

void GameClearScene::Result2Update()
{
	auto& input = Input::GetInstance();
	bool isAllMax = true;
	for (auto& scoreUI : m_scoreUIList)
	{
		if (!scoreUI.lock()->IsViewScoreMax())
		{
			isAllMax = false;
			break;
		}
	}
	//�S�ẴX�R�AUI���ő�l�ɂȂ�����
	if (isAllMax)
	{
		//�~�߂�
		m_addScoreSE.lock()->Stop();
		//A�{�^���Ŏ���
		if (input.IsTrigger("A"))
		{
			auto& fader = Fader::GetInstance();
			//���񂾂�Â�
			fader.FadeOut();
			//���̏��
			m_update = &GameClearScene::DisappearUpdate;
			return;
		}
	}
}

void GameClearScene::DisappearUpdate()
{
	auto& fader = Fader::GetInstance();
	//�Â��Ȃ�����
	if (fader.IsFinishFadeOut())
	{
		//�����̉��ɂȂ��Ă�V�[����؂�ւ���
		m_controller.ChangeBaseScene(std::make_shared<SelectStageScene>(m_controller,m_stageIndex));
		m_controller.PopScene();//�����͏�����
		return;
	}
}

void GameClearScene::NormalDraw()
{
	//�w�i�𔒂��ۂ�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlphaRate * m_countFrame);
	DrawBox(0, 0,//����
		Game::kScreenWidth, Game::kScreenHeight,//�E��
		0xffffff,//�J���[
		true);//�h��ׂ�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameClearScene::InitResult1UI()
{
	//���[�_�[
	auto loader = std::make_shared<CSVDataLoader>();
	m_scoreUiData = loader->LoadResultScoreUIDataCSV();
	//�X�R�A
	auto score = SaveDataManager::GetInstance().GetScore().lock();

	//�f�[�^�̐�����UI��p��
	for (auto& data : m_scoreUiData)
	{
		std::shared_ptr<ResultScoreUI> scoreUI;
		float value = 0.0f;
		if (data.name == "TotalScore")
		{
			value = score->GetScore();
		}
		else if (data.name == "KillScore")
		{
			value = score->GetKillScore();
		}
		else if (data.name == "ItemScore")
		{
			value = score->GetItemScore();
		}
		else if (data.name == "TimeScore")
		{
			value = score->GetTimeScore();
		}
		else if (data.name == "HPScore")
		{
			value = score->GetHPScore();
		}
		//�f�[�^�ɂȂ����͔̂�΂�
		else
		{
			continue;
		}
		scoreUI = std::make_shared<ResultScoreUI>(value, data.pos, data.text);
		scoreUI->Init();
		m_scoreUIList.emplace_back(scoreUI);
	}
}

void GameClearScene::InitResult2UI()
{
	//����UI���폜
	for (auto& scoreUI : m_scoreUIList)
	{
		if (auto ui = scoreUI.lock())
		{
			//�폜
			ui->Delete();
		}
	}
	m_scoreUIList.clear();
	//�X�R�A
	auto score = SaveDataManager::GetInstance().GetScore().lock();

	//�f�[�^�̐�����UI��p��
	for (auto& data : m_scoreUiData)
	{
		std::shared_ptr<ResultScoreUI> scoreUI;
		float value = 0.0f;
		if (data.name == "HighScore1")
		{
			value = score->GetHighScore(m_stageIndex)[0];
		}
		else if (data.name == "HighScore2")
		{
			value = score->GetHighScore(m_stageIndex)[1];
		}
		else if (data.name == "HighScore3")
		{
			value = score->GetHighScore(m_stageIndex)[2];
		}
		//�f�[�^�ɂȂ����͔̂�΂�
		else
		{
			continue;
		}
		scoreUI = std::make_shared<ResultRankingScore>(value, data.pos, data.text, score->GetScore());
		scoreUI->Init();
		m_scoreUIList.emplace_back(scoreUI);
	}
}
