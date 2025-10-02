#include "SelectStagePlayer.h"
#include <DxLib.h>
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Rigidbody.h"

namespace
{
	//ダンス
	const char* kDance1Anim = "Select|Dance1";
	const char* kDance2Anim = "Select|Dance2";
	const char* kDance3Anim = "Select|Dance3";
	//ダンスの数
	constexpr int kDanceNum = 3;
	//走る
	const char* kRunAnim = "Select|Run";
	//決定
	const char* kDecideAnim = "Select|Decide";
	//lerp率
	constexpr float kLerpRate = 0.05f;
	//目的地に到達したとみなす範囲
	constexpr float kTargetReachRadius = 100.0f;
}

SelectStagePlayer::SelectStagePlayer(Vector3 cameraPos, Vector3 pos):
	CharacterBase(Shape::None),
	m_isChangeDance(false)
{
	//モデル
	m_model = std::make_shared<Model>(MV1LoadModel(L"Data/Model/Player/SelectStage/SelectStagePlayer.mv1"), Vector3::Zero().ToDxLibVector());
	//初期位置
	m_rb->m_pos = pos;
	m_model->SetPos(pos.ToDxLibVector());
	//カメラ方向を向かせる
	Vector3 dir = (cameraPos - m_rb->m_pos);
	m_model->SetDir(dir.XZ());
}

SelectStagePlayer::~SelectStagePlayer()
{
}

void SelectStagePlayer::Init()
{
	//アニメーション
	m_model->SetAnim(kDance3Anim, true);
	//フラグをtrueに
	m_isChangeDance = true;
}

void SelectStagePlayer::Update(Vector3 cameraPos, Vector3 targetPos)
{
	auto& input = Input::GetInstance();
	if (input.IsTrigger("A") && !m_isDecided)
	{
		//カメラ方向を向かせる
		Vector3 dir = (cameraPos - m_rb->m_pos);
		m_model->SetDir(dir.XZ());
		//決定
		m_model->SetAnim(kDecideAnim, false);
		m_isDecided = true;
	}
	//アニメーションの更新
	m_model->Update();
	//だんだん目的地に移動
	m_rb->m_pos = Vector3::Lerp(m_rb->m_pos, targetPos, kLerpRate);
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
	if (!m_isDecided)
	{
		if ((m_rb->m_pos - targetPos).Magnitude() > kTargetReachRadius)
		{
			//進行方向にモデルを向かせる
			Vector3 dir = (targetPos - m_rb->m_pos);
			m_model->SetDir(dir.XZ());
			//アニメーション
			m_model->SetAnim(kRunAnim, true);
			m_isChangeDance = false;
		}
		else if (!m_isChangeDance)
		{
			//カメラ方向を向かせる
			Vector3 dir = (cameraPos - m_rb->m_pos);
			m_model->SetDir(dir.XZ());
			//ランダムに踊りを決める
			switch (GetRand(kDanceNum) % kDanceNum + 1)
			{
			case 1:
				m_model->SetAnim(kDance1Anim, true);
				break;
			case 2:
				m_model->SetAnim(kDance2Anim, true);
				break;
			case 3:
				m_model->SetAnim(kDance3Anim, true);
				break;
			}
			m_isChangeDance = true;
		}
	}
}

void SelectStagePlayer::Draw() const
{
	//モデルを描画
	m_model->Draw();
}

void SelectStagePlayer::End()
{
	//モデルの終了処理
	m_model->End();
}

void SelectStagePlayer::InitSound()
{
}
