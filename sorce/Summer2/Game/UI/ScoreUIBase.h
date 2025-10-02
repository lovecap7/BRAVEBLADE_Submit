#pragma once
#include "UIBase.h"
#include <memory>
#include "../../General/Math/MyMath.h"

namespace
{
    //スコアの桁数
    constexpr int kDigitNum = 6;
}

class Score;
class ScoreUIBase abstract:
	public UIBase
{
public:
    ScoreUIBase(Vector2 basePos,float scale,float digitMargin);
    virtual ~ScoreUIBase();
    virtual void Draw() const override;
    bool IsViewScoreMax() const { return m_viewScore >= m_viewMaxScore; };
protected:
    //描画スコアの更新
    void UpdateViewScore(int viewScore);
    //スコアを直接変更する
    void SetViewScore(int viewScore) { m_viewScore = viewScore; };
    //描画用スコア
    int m_viewScore;	//現在表示しているスコア
    int m_viewMaxScore;	//現在の最大スコア
    //基準座標
    Vector2 m_basePos;
    //大きさ
    float m_scale;
    //1桁の幅
    float m_digitMargin;
    //スコアが変動するたびにはねる
    Vector2 m_viewPoses[kDigitNum];
    Vector2 m_viewVecs[kDigitNum];
    //各位の値
    int m_digits[kDigitNum];
    //ハンドル
    int m_handle;
    //スコアが更新されたら跳ねる
    void BounceScore(int jumpPower, int groundY);
};
