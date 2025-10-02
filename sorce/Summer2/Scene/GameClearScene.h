#pragma once
#include "SceneBase.h"
#include "../General/StageIndex.h"
#include <memory>
#include <vector>
#include "../General/CSVDataLoader.h"
class SceneController;
class ResultScoreUI;
class SE;
class GameClearScene :
    public SceneBase
{
public:
    GameClearScene(SceneController& controller, Stage::StageIndex index);
    ~GameClearScene();
    //�h���N���X�Ŏ���������
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
private:
    //��ԑJ��
    using UpdateFunc_t = void(GameClearScene::*)();
    using DrawFunc_t = void(GameClearScene::*)();
    UpdateFunc_t m_update;
    DrawFunc_t m_draw;
    //�o�������
    void AppearUpdate();
    //���U���g�P���
    void Result1Update();
    //���U���g2���
    void Result2Update();
    //���Œ����
    void DisappearUpdate();
    //�ʏ�`��
    void NormalDraw();
    //UI�̏���
    void InitResult1UI();
    void InitResult2UI();
private:
    int m_countFrame;
    //�X�e�[�W�̔ԍ�
    Stage::StageIndex m_stageIndex;
    //�X�R�AUI�̃f�[�^
    std::vector<ResultScoreUIData> m_scoreUiData;
    //�X�R�AUI
	std::vector<std::weak_ptr<ResultScoreUI>> m_scoreUIList;
    //���ZSE
    std::weak_ptr<SE> m_addScoreSE;
};

