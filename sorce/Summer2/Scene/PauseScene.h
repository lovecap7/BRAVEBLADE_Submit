#pragma once
#include "SceneBase.h"
#include "../General/StageIndex.h"
#include <memory>
#include <map>
class SceneController;
class MenuUI;
class Input;
class PauseScene :
    public SceneBase
{
public:
    PauseScene(SceneController& controller,Stage::StageIndex index);
    ~PauseScene();
    //�h���N���X�Ŏ���������
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
private:
    //��ԑJ��
    using UpdateFunc_t = void(PauseScene::*)();
    using DrawFunc_t = void(PauseScene::*)();
    UpdateFunc_t m_update;
    DrawFunc_t m_draw;
    //�o�������
    void AppearUpdate();
    //�m�[�}�����
    void NormalUpdate();
    //���Œ����
    void DisappearUpdate();
    //�ʏ�`��
    void NormalDraw();
    //�r���`��
    void ShiftingDraw();
    //�Q�[���ɖ߂�
    void RetrunGame();
    //�Q�[�����ăX�^�[�g
    void RestartGame();
    //�I�v�V����
    void Option();
    //�Z���N�g�V�[����
    void SelectStage();
private:
    int m_countFrame;
    enum class MenuIndex : int 
    {
        RetrunGame  = 1,
        RestartGame = 2,
        Option      = 3,
        SelectStage = 4
    };
    //���j���[�Z���N�g
    MenuIndex m_menuSelectIndex;
    //�e���j���[UI
    std::weak_ptr<MenuUI> m_pauseUI;
    std::map<MenuIndex,std::weak_ptr<MenuUI>> m_menuUIs;
    //������
    void InitDisappear();
    void InitUI();
    //���j���[�I��
    void MenuSelect(Input& input);
    //�X�e�[�W�C���f�b�N�X
	Stage::StageIndex m_stageIndex;
};

