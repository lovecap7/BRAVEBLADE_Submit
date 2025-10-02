#pragma once
#include "UIBase.h"
#include <memory>
#include "../../General/Math/MyMath.h"

namespace
{
    //�X�R�A�̌���
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
    //�`��X�R�A�̍X�V
    void UpdateViewScore(int viewScore);
    //�X�R�A�𒼐ڕύX����
    void SetViewScore(int viewScore) { m_viewScore = viewScore; };
    //�`��p�X�R�A
    int m_viewScore;	//���ݕ\�����Ă���X�R�A
    int m_viewMaxScore;	//���݂̍ő�X�R�A
    //����W
    Vector2 m_basePos;
    //�傫��
    float m_scale;
    //1���̕�
    float m_digitMargin;
    //�X�R�A���ϓ����邽�тɂ͂˂�
    Vector2 m_viewPoses[kDigitNum];
    Vector2 m_viewVecs[kDigitNum];
    //�e�ʂ̒l
    int m_digits[kDigitNum];
    //�n���h��
    int m_handle;
    //�X�R�A���X�V���ꂽ�璵�˂�
    void BounceScore(int jumpPower, int groundY);
};
