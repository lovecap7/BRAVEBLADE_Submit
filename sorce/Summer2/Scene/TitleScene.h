#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
#include <map>
#include <string>
class Input;
class TitleCamera;
class SceneController;
class Model;
class TitleUI;
class TitlePlayer;
class Actor;
class CSVDataLoader;
class MenuUI;
class SaveDataUI;
class DialogUI;

namespace
{
    constexpr int kMenuNum = 4;
}

class TitleScene :
    public SceneBase
{
public:
    TitleScene(SceneController& controller);
    ~TitleScene();
    /// <summary>
    /// �h���N���X�Ŏ���������
    /// virtual �͂�����p�����邩������Ȃ�����
    /// override�͖����I�ɃI�[�o�[���C�h���G�f�B�^�Ɏ�������
    /// </summary>
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override {};
    //�n���h�����[�h
    void LoadHandle();
    //�z�u
    void LoadStage(std::shared_ptr<CSVDataLoader> csvLoader);
    //�n���h���폜
    void AllDeleteHandle();
    //�I�u�W�F�폜
    void AllDeleteStage();
private:
    //�J����
	std::unique_ptr<TitleCamera> m_camera;
    //�v���C���[
	std::shared_ptr<TitlePlayer> m_player;
    //�`��p�I�u�W�F
    std::vector<std::shared_ptr<Actor>> m_stageObjects;
    //�n���h��
    std::map<std::string, int> m_stageHandles;
    //�e
    int m_shadowMapHandle;
    //���C�g�n���h��
    int m_lightHandle;
    enum class MenuIndex : int
    {
        Continue = 0,
        NewGame = 1,
        Option = 2,
        FinishGame = 3
    };
    //�^�C�g��UI
    std::weak_ptr<TitleUI> m_titleUI;
    //���j���[UI
    std::map<MenuIndex,std::weak_ptr<MenuUI>> m_menuUIs;
    //�Z�[�u�f�[�^UI
    std::weak_ptr<SaveDataUI> m_saveDataUI;
    //�_�C�����O
    std::weak_ptr<DialogUI> m_dialogUI;
    //���j���[�C���f�b�N�X
    MenuIndex m_menuIndex;
    //����
    bool m_isDecide;
private:
    //��ԑJ��
    using UpdateFunc_t = void(TitleScene::*)(Input& input);
    UpdateFunc_t m_update;
    //�^�C�g���݂̂̍X�V
    void UpdateTitle(Input& input);
    //�Z���N�g���
    void UpdateSelectMenu(Input& input);
    //�_�C�A���O
    void UpdateDialog(Input& input);
private:
    //���j���[�Z���N�g����
    void SelectMenu(Input& input);
    //��������
    void InitContinue();
    void Continue();
    //���߂���
    void InitNewGame();
    void NewGame();
    //�ݒ�
    void Option();
    //�Q�[���I��
    void InitFinishGame();
    void FinishGame();
private:
    //���C�g�̏�����
    void InitLight();
    //�V���h�E�}�b�v�̏�����
    void InitShadow();
    //�V���h�E�̍X�V
    void UpdateShadow();
    //Actor�̍X�V
    void UpdateCommon();
    //UI�̏����A������
    void InitUIs(std::shared_ptr<CSVDataLoader>& csvLoader);
    //�^�C�g����ʂ̏�����
    void InitTitle();
    //���j���[��ʂ̏�����
    void InitSelectMenu();

};

