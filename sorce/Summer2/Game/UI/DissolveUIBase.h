#pragma once
#include "UIBase.h"
#include "../../General/Math/MyMath.h"
class DissolveUIBase abstract:
    public UIBase
{
public:
	DissolveUIBase(int mainH,int shaderH,int patternH, bool isAppear);
	virtual ~DissolveUIBase();
	//�o�����������Ȃ�
	bool IsAppered()const;
	//�����Ă�Ȃ�
	bool IsDisappeared()const;
	//����
	void DissolveRateMax();
	void DissolveRateMin();
protected:
	void Update(float dissolveSpeed, bool isAppear);
	void Draw(Vector2 pos)const;
	//���C���̕`��n���h��
	int m_mainHandle;
	//�f�B�]���u�Ɏg���ϐ�
	//�p�^�[��
	int m_patternHandle;
	//�V�F�[�_
	int m_pixelShaderHandle;
	//�V�F�[�_�̒萔�o�b�t�@
	int m_constantBufferHandle = -1;
	//�V�F�[�_�̃p�����[�^
	float* m_threshold;
	float m_dissolveRate;
};

