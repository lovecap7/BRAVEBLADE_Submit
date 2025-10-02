#pragma once
#include "../UIBase.h"
#include "../../../General/CSVDatas.h"
#include "../../../General/StageIndex.h"
#include "../../../General/Math/MyMath.h"
#include <array>
#include <map>
class SelectStageRankingUI :
    public UIBase
{
public:
    SelectStageRankingUI(Stage::StageIndex index);
    virtual ~SelectStageRankingUI();
    void Update()override;
    void Draw()const override;
    void SetStageIndex(Stage::StageIndex index);
private:
    //�n���h��
    int m_textHandle;
	//�X�e�[�W�̃C���f�b�N�X
	Stage::StageIndex m_stageIndex;
    //�X�R�A�ƃC���f�b�N�X�̘A�z�z��
    std::map<Stage::StageIndex, std::array<int, static_cast<int>(Stage::StageIndex::StageNum)>> m_stageScore;
};

