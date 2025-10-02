#include "DissolveUIBase.h"
#include "../../Shader/MyDraw.h"
#include <cassert>

namespace
{
	//�f�B�]���u�̍ő�l
	constexpr float kMaxDissolveRate = 1.5f;  //���S�ɏ�������
	//�f�B�]���u�̍ŏ��l
	constexpr float kMinDissolveRate = 0.0f; //���S�ɏo��������
}

DissolveUIBase::DissolveUIBase(int mainH, int shaderH, int patternH, bool isAppear) :
	UIBase(),
	m_mainHandle(mainH),
	m_pixelShaderHandle(shaderH),
	m_patternHandle(patternH)
{
	//�o�����Ă���Ȃ�
	if (isAppear)
	{
		m_dissolveRate = kMinDissolveRate;
	}
	//�o�����Ă��Ȃ��Ȃ�
	else
	{
		m_dissolveRate = kMaxDissolveRate;
	}
	assert(m_pixelShaderHandle != -1);
	assert(m_mainHandle != -1);
	assert(m_patternHandle != -1);
	//�萔�o�b�t�@�̊m��
	m_constantBufferHandle = DxLib::CreateShaderConstantBuffer(sizeof(float) * 4);//16�A���C�����g
	//�O���{�̃������͎��͒��ڂ�����Ȃ��B
	//RAM��̃������ŃV�~�����[�g���Ă�
	m_threshold = static_cast<float*>(DxLib::GetBufferShaderConstantBuffer(m_constantBufferHandle));
}

DissolveUIBase::~DissolveUIBase()
{
	DeleteGraph(m_mainHandle);
	DeleteGraph(m_patternHandle);
	DeleteShader(m_pixelShaderHandle);
}

bool DissolveUIBase::IsAppered() const
{
	return m_dissolveRate <= kMinDissolveRate;
}

bool DissolveUIBase::IsDisappeared() const
{
	return m_dissolveRate >= kMaxDissolveRate;
}

void DissolveUIBase::DissolveRateMax()
{
	m_dissolveRate = kMaxDissolveRate;
}

void DissolveUIBase::DissolveRateMin()
{
	m_dissolveRate = kMinDissolveRate;
}

void DissolveUIBase::Update(float dissolveSpeed, bool isAppear)
{
	//�o��
	if (isAppear)
	{
		m_dissolveRate -= dissolveSpeed;
	}
	else
	{
		m_dissolveRate += dissolveSpeed;
	}
	m_dissolveRate = MathSub::ClampFloat(m_dissolveRate, kMinDissolveRate, kMaxDissolveRate);
	m_threshold[0] = m_dissolveRate;
	UpdateShaderConstantBuffer(m_constantBufferHandle);
}

void DissolveUIBase::Draw(Vector2 pos) const
{
	//�Ō�̔ԍ��̃��W�X�^�ɑ�����
	SetShaderConstantBuffer(m_constantBufferHandle, DX_SHADERTYPE_PIXEL, 4);
	//�V�F�[�_�ŕ`��
	MyDrawUtils::DrawRotaGraph(
		pos,
		1.0f,
		m_dissolveRate,
		m_mainHandle,
		m_pixelShaderHandle,
		{ m_patternHandle }
	);
}
