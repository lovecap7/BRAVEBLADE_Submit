#pragma once
#include "SceneBase.h"
#include "../General/StageIndex.h"
#include <memory>
#include <map>
class SceneController;
class MenuUI;
class Input;
class GameoverScene :
    public SceneBase
{
public:
    GameoverScene(SceneController& controller, Stage::StageIndex index);
    ~GameoverScene();
    //�h���N���X�Ŏ���������
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
private:
    //��ԑJ��
    using UpdateFunc_t = void(GameoverScene::*)();
    using DrawFunc_t = void(GameoverScene::*)();
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
    //UI������
    void InitUI();
    //���j���[�Z���N�g
    void MenuSelect(Input& input);
    //�Â�����
    void Continue();
    //�ăX�^�[�g
    void RestartBase();
    //�X�e�[�W�Z���N�g��
    void SelectStage();
private:
    int m_countFrame;
    enum class MenuIndex : int
    {
        Continue    = 1,
        Restart     = 2,
        SelectStage = 3
    };
    //���j���[�Z���N�g
    MenuIndex m_menuSelectIndex;
    //�e���j���[UI
    std::weak_ptr<MenuUI> m_gameoverUI;
    std::map<MenuIndex, std::weak_ptr<MenuUI>> m_menuUIs;
	//�X�e�[�W�̔ԍ�
    Stage::StageIndex m_stageIndex;
};

