#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
//�A�N�^�[�����G�ꏊ�̒��ɂ��邩�`�F�b�N����
class SearchPlace 
{
public:
	SearchPlace(Vector3 pos,float rang);
	~SearchPlace();
	//�f�o�b�O�p
	void Draw()const;
	//�͈͓��ɂ��邩
	bool IsInSearchPlace(Vector3 targetPos)const;
	//���W
	Vector3 GetPos()const { return m_pos; };
	//�͈�
	void SetRange(float range) { m_range = range; };
	float GetRange() const{ return m_range; };
private:
	//�͈�
	float m_range;
	//���W
	Vector3 m_pos;
};

