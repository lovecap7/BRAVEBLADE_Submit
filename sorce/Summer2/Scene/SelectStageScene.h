#pragma once
#include "SceneBase.h"
#include <memory>
#include <map>
#include <string>
#include <array>
#include "../General/Math/MyMath.h"
#include "../General/StageIndex.h"
class Input;
class SelectStagePlayer;
class SelectStageCamera;
class SceneController;
class StageFigure;
class SelectStageBackUI;
class SelectStageRankingUI;
class LeftArrowUI;
class SelectStageScene :
    public SceneBase
{
public:
    SelectStageScene(SceneController& controller, Stage::StageIndex index);
    ~SelectStageScene();
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
    //�X�e�[�W�Z���N�g
    void SelectStageIndex(Input& input);
private:
    //����
    bool m_isDecide;
    //�J����
    std::unique_ptr<SelectStageCamera> m_camera;
    //���f��
    std::unique_ptr<SelectStagePlayer> m_player;
    //�X�e�[�W�̏ꏊ
    std::map<Stage::StageIndex,Vector3> m_stagePos;
    //�X�e�[�W�̃C���f�b�N�X
    int m_stageIndex;
    //�X�e�[�W�̃t�B�M���A
	std::array<std::shared_ptr<StageFigure>,static_cast<int>(Stage::StageIndex::StageNum)> m_stageFigures;
    //�w�iUI
    std::weak_ptr<SelectStageBackUI> m_stage1Back;
    std::weak_ptr<SelectStageBackUI> m_stage2Back;
	//�����L���OUI
    std::weak_ptr<SelectStageRankingUI> m_stageRankingUI;
	//���UI
	std::weak_ptr<LeftArrowUI> m_leftArrowUI;
	std::weak_ptr<LeftArrowUI> m_rightArrowUI;
    //������Ă���ŏI�X�e�[�W�̃C���f�b�N�X
    Stage::StageIndex m_unlockStageIndex;
    //�w�i��ς���
    void ChangeBack();
    //����������Ă���X�e�[�W�܂ł����I�ׂȂ����邽�߂̏���
    void NowUnlockStage();
};

