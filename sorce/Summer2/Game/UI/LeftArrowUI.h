#pragma once
#include "UIBase.h"
#include "../../General/Math/MyMath.h"
class LeftArrowUI :
    public UIBase
{
public:
    LeftArrowUI(Vector2 pos,bool isRight);
    ~LeftArrowUI();
    void Update() override;
    void Draw() const override;
	void SetIsSelect(bool isSelect) { m_isSelect = isSelect; };
	void SetPos(Vector2 pos) { m_pos = pos; };
	void SetBaseScale(double baseScale) { m_baseScale = baseScale; }
	void SetIsLock(bool isLock) { m_isLock = isLock; };
private:
    //選択してるか
	bool m_isSelect;
    //拡大縮小
    double m_scale;
    //基準の大きさ
    double m_baseScale;
    //拡大縮小に使う
    float m_scaleAngle;
	//位置
	Vector2 m_pos;
	//矢印ハンドル
	int m_arrowHandle;
    //鍵付き
    int m_lockArrowHandle;
    //右
	bool m_isRight;
    //鍵がかかってる
	bool m_isLock;
};

