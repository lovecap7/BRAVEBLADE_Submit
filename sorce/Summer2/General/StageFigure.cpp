#include "StageFigure.h"
#include "../General/Model.h"

namespace
{
	//モデルの位置調整
	const Vector3 kModelOffset = { 0.0f, 0.0f, 500.0f }; // モデルの位置を調整するオフセット
	//回転
	constexpr float kRotationSpeed = 0.5f; // 回転速度
}

StageFigure::StageFigure()
{
}

StageFigure::~StageFigure()
{
}

void StageFigure::Init(int handle, Vector3 pos)
{
	auto newPos = pos + kModelOffset; // モデルの位置を調整
	//モデルの初期化
	m_model = std::make_shared<Model>(handle, newPos.ToDxLibVector());
}

void StageFigure::Update()
{
	//アニメーションの更新
	m_model->Update();
	//モデルの回転
	m_model->SetRot(VGet(0.0f, kRotationSpeed, 0.0f));
}

void StageFigure::Draw() const
{
	//モデルを描画
	m_model->Draw();
}


void StageFigure::End()
{
	//モデルの終了処理
	m_model->End();
}
