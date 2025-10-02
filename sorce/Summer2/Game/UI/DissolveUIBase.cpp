#include "DissolveUIBase.h"
#include "../../Shader/MyDraw.h"
#include <cassert>

namespace
{
	//ディゾルブの最大値
	constexpr float kMaxDissolveRate = 1.5f;  //完全に消える状態
	//ディゾルブの最小値
	constexpr float kMinDissolveRate = 0.0f; //完全に出現する状態
}

DissolveUIBase::DissolveUIBase(int mainH, int shaderH, int patternH, bool isAppear) :
	UIBase(),
	m_mainHandle(mainH),
	m_pixelShaderHandle(shaderH),
	m_patternHandle(patternH)
{
	//出現しているなら
	if (isAppear)
	{
		m_dissolveRate = kMinDissolveRate;
	}
	//出現していないなら
	else
	{
		m_dissolveRate = kMaxDissolveRate;
	}
	assert(m_pixelShaderHandle != -1);
	assert(m_mainHandle != -1);
	assert(m_patternHandle != -1);
	//定数バッファの確保
	m_constantBufferHandle = DxLib::CreateShaderConstantBuffer(sizeof(float) * 4);//16アライメント
	//グラボのメモリは実は直接いじれない。
	//RAM上のメモリでシミュレートしてる
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
	//出現
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
	//最後の番号のレジスタに送られる
	SetShaderConstantBuffer(m_constantBufferHandle, DX_SHADERTYPE_PIXEL, 4);
	//シェーダで描画
	MyDrawUtils::DrawRotaGraph(
		pos,
		1.0f,
		m_dissolveRate,
		m_mainHandle,
		m_pixelShaderHandle,
		{ m_patternHandle }
	);
}
