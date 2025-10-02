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
    //ハンドル
    int m_textHandle;
	//ステージのインデックス
	Stage::StageIndex m_stageIndex;
    //スコアとインデックスの連想配列
    std::map<Stage::StageIndex, std::array<int, static_cast<int>(Stage::StageIndex::StageNum)>> m_stageScore;
};

