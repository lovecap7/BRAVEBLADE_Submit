#pragma once
#include "UIBase.h"
#include "../../General/Math/MyMath.h"
class DissolveUIBase abstract:
    public UIBase
{
public:
	DissolveUIBase(int mainH,int shaderH,int patternH, bool isAppear);
	virtual ~DissolveUIBase();
	//出現しきったなら
	bool IsAppered()const;
	//消えてるなら
	bool IsDisappeared()const;
	//割合
	void DissolveRateMax();
	void DissolveRateMin();
protected:
	void Update(float dissolveSpeed, bool isAppear);
	void Draw(Vector2 pos)const;
	//メインの描画ハンドル
	int m_mainHandle;
	//ディゾルブに使う変数
	//パターン
	int m_patternHandle;
	//シェーダ
	int m_pixelShaderHandle;
	//シェーダの定数バッファ
	int m_constantBufferHandle = -1;
	//シェーダのパラメータ
	float* m_threshold;
	float m_dissolveRate;
};

