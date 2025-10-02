#pragma once
#include "SceneBase.h"
#include "../General/StageIndex.h"
#include <vector>
#include <memory>

class SceneController;
class Input;
class GameManager;
class StageScene :
    public SceneBase
{
public:
    StageScene(SceneController& controller, Stage::StageIndex index = Stage::StageIndex::Stage1);
    ~StageScene();
    /// <summary>
    /// �h���N���X�Ŏ���������
    /// virtual �͂�����p�����邩������Ȃ�����
    /// override�͖����I�ɃI�[�o�[���C�h���G�f�B�^�Ɏ�������
    /// </summary>
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
    void Continue();
private:
    //�Q�[���}�l�[�W���[
    std::unique_ptr<GameManager> m_gameManager;
    //�X�e�[�W�ԍ�
    Stage::StageIndex m_stageIndex;
	//BGM������
    void InitBGM();
};

