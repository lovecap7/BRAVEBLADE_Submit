#include "Application.h"
#include <DxLib.h>
#include "../General/game.h"
#include "../Scene/SceneController.h"
#include  "../General/Input.h"
#include  "../General/Collision/Physics.h"
#include  "../General/Effect/EffekseerManager.h"
#include  "../Game/UI/UIManager.h"
#include  "../General/Fader.h"
#include  "../General/Sound/SoundManager.h"
#include  "../SaveData/SaveDataManager.h"
#include  "../General/LoadingManager.h"
#include  <cassert>

Application& Application::GetInstance()
{
    //���̎��_�Ń��������m�ۂ���ăA�v���I���܂Ŏc��
    static Application app;
    return app;
}

bool Application::Init()
{
	//������
	//���̃t���O��true�̎��A�v���P�[�V�������I������
	m_isFinishApplication = false;

	//�Q�[���^�C�g��
	SetWindowText(L"BRAVE BLADE");
	//�Q�[���A�C�R��
	//SetWindowIconID(IDI_ICON1);

	 //�t���X�N���[���łȂ��A�E�B���h�E���[�h�ŊJ���悤�ɂ���
	//�����������֐��̓E�B���h�E���J���O��(Dxlib.Init()�̑O)�ɏ������Ă����K�v������
	ChangeWindowMode(m_isWindow);

	// DirectX11���g�p����悤�ɂ���B(DirectX9���A�ꕔ�@�\�s��)
	// Effekseer���g�p����ɂ͕K���ݒ肷��B
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	//��ʃT�C�Y�ύX
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum);
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return false;			// �G���[���N�����璼���ɏI��
	}

	//�`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	//3D�`��̂��߂̏���
	//Z�o�b�t�@���g�p����
	SetUseZBuffer3D(true);
	//Z�o�b�t�@�ɏ������݂��s��
	SetWriteZBuffer3D(true);
	//�o�b�N�J�����O���s��(�|���S���̗��ʂ�`�悵�Ȃ��悤�ɂ���)
	SetUseBackCulling(true);

	//Physics�̏�����
	Physics::GetInstance().Init();
	//�G�t�F�N�g�̏�����
	EffekseerManager::GetInstance().Init();

    return true;
}

void Application::Run()
{
	//���[�f�B���O
	auto& loadingManager = LoadingManager::GetInstance();
	//�񓯊����[�h�J�n
	loadingManager.StartLoading();
	//�R���g���[���[
	auto& input = Input::GetInstance();
	input.Init();
	//Physics(�Փˏ���)
	auto& physics = Physics::GetInstance();
	//�Z�[�u�f�[�^
	auto& saveDataManager = SaveDataManager::GetInstance();
	saveDataManager.Init();
	//�T�E���h
	auto& soundManager = SoundManager::GetInstance();
	soundManager.Init();
	//�G�t�F�N�g
	auto& effect = EffekseerManager::GetInstance();
	//UI�}�l�[�W���[
	auto& uiManager = UIManager::GetInstance();
	uiManager.Init();
	//�t�F�[�h
	auto& fader = Fader::GetInstance();
	fader.Init();
	//�V�[��
	SceneController* sceneController = new SceneController();
	//���[�f�B���O������
	loadingManager.Init();
	//�񓯊��I��
	loadingManager.StopLoading();

	//�Q�[�����[�v
	while (ProcessMessage() != -1) // Windows���s��������҂�
	{
		//����̃��[�v���n�܂������Ԃ��o���Ă���
		LONGLONG time = GetNowHiPerformanceCount();

		//��ʑS�̂��N���A
		ClearDrawScreen();

		//�����ɃQ�[���̏���������
		//Widow���[�h���؂�ւ���������`�F�b�N
		bool isWindow = m_isWindow;
		
		//�X�V
		//���[�h���͍X�V���~�߂�
		bool isLoading = loadingManager.IsLoading();
		soundManager.Update();
		saveDataManager.Update();
		if (!isLoading)
		{
			input.Update();
			sceneController->Update();
			physics.Update();
			effect.Update();
			uiManager.Update();
			fader.Update();
		}
		loadingManager.Update();
		//���[�h���͕`����~�߂�
		if (!isLoading)
		{
			uiManager.BackDraw();
			sceneController->Draw();
			effect.Draw();
			uiManager.FrontDraw();
			fader.Draw();
		}
		//�`��
		loadingManager.Draw();

#if _DEBUG
		DrawFormatString(0, 500, 0xff0000, L"FPS : %.2f", GetFPS());
#endif
		//�؂�ւ�����Ȃ�
		if (m_isWindow != isWindow)
		{
			//�؂�ւ�菈��
			ChangeScreenMode();
		}

		//��ʂ̐؂�ւ���҂K�v������
		ScreenFlip();//1/60�b�o�߂���܂ő҂�

		//FPS��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667);
		{

		}

		//ESC�L�[�ŏI��
		if (CheckHitKey(KEY_INPUT_ESCAPE) || m_isFinishApplication)
		{
			sceneController = nullptr;
			break;
		}
	}
	delete sceneController;
	sceneController = nullptr;
}

void Application::Terminate()
{
	Physics::GetInstance().Reset();
	EffekseerManager::GetInstance().End();
	UIManager::GetInstance().End();
	SoundManager::GetInstance().End();
	SaveDataManager::GetInstance().End();
	LoadingManager::GetInstance().End();
	DxLib_End();				// �c�w���C�u�����g�p�̏I������
}

void Application::SetWindowMode(bool isWindow)
{
	m_isWindow = isWindow;
}

void Application::ChangeScreenMode()
{
	//��ʃ��[�h�ύX��(�ƃE�C���h�E���[�h�ύX�� )�ɃO���t�B�b�N�X�V�X�e���̐ݒ��O���t�B�b�N�n���h�������Z�b�g���邩�ǂ�����ݒ肷��
	//Flag TRUE:���Z�b�g����(�f�t�H���g)FALSE:���Z�b�g���Ȃ�
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	//�𑜓x
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum);
	//�؂�ւ�
	ChangeWindowMode(m_isWindow);
	// ���ۂ̃E�B���h�E�T�C�Y���ݒ�i�t���[�����݁j
	SetWindowSize(Game::kScreenWidth, Game::kScreenHeight);
	//�Ċg�嗦��1�{�ɖ߂�
	SetWindowSizeExtendRate(1.0);
	//��ʑS�̂��N���A
	ClearDrawScreen();
}
