#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
//アクターが索敵場所の中にいるかチェックする
class SearchPlace 
{
public:
	SearchPlace(Vector3 pos,float rang);
	~SearchPlace();
	//デバッグ用
	void Draw()const;
	//範囲内にいるか
	bool IsInSearchPlace(Vector3 targetPos)const;
	//座標
	Vector3 GetPos()const { return m_pos; };
	//範囲
	void SetRange(float range) { m_range = range; };
	float GetRange() const{ return m_range; };
private:
	//範囲
	float m_range;
	//座標
	Vector3 m_pos;
};

