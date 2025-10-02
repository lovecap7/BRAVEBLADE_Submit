#include "ScoreUIBase.h"
#include "../../General/game.h"
#include "../GameRule/Score.h"
#include "../UI/UIManager.h"
#include <DxLib.h>

namespace
{
	//加算の基本値
	constexpr int kAddBaseSpeed = 128;
	//差の割合
	constexpr float kAddDiffRate = 0.1f;
	//減速割合
	constexpr int kAddLowSpeedRate = 6;
	//減速する差
	constexpr int kChangeLowSpeed = 300;
	//幅
	constexpr int kImageWidth = 256;
	constexpr int kImageHeight = 256;
	//重力
	constexpr float kGravity = 1.0f;
	//ジャンプ力
	constexpr float kJumpPower = 2.0f;
}

ScoreUIBase::ScoreUIBase(Vector2 basePos, float scale, float digitMargin):
	UIBase(),
	m_basePos(basePos),
	m_scale(scale),
	m_digitMargin(digitMargin),
	m_viewScore(0),
	m_viewMaxScore(0),
	m_digits{},
	m_viewVecs{},
	m_handle(UIManager::GetInstance().GetImageHandle("Score"))
{
	for (auto& pos : m_viewPoses)
	{
		pos = m_basePos;
	}
}

ScoreUIBase::~ScoreUIBase()
{
}

void ScoreUIBase::Draw() const
{
	if (!m_isDraw)return;
	for (int i = 0;i < kDigitNum;++i)
	{
		//切り取りを計算する
		int sizeX, sizeY;
		GetGraphSize(m_handle, &sizeX, &sizeY);//画像サイズ
		int cutX = m_digits[i] % (sizeX / kImageWidth);//横
		int cutY = m_digits[i] / (sizeX / kImageWidth);//縦
		//描画
		DrawRectRotaGraphFast(m_viewPoses[i].x - i * m_digitMargin, m_viewPoses[i].y,
			kImageWidth * cutX,
			kImageHeight * cutY,
			kImageWidth, kImageHeight,
			m_scale, 0.0f, m_handle, true, false);
	}
}

void ScoreUIBase::UpdateViewScore(int viewScore)
{
	m_viewMaxScore = viewScore;
	if (m_viewMaxScore <= 0)
	{
		m_viewScore = 0;
	}
	else
	{
		//現在のスコアと目標スコアの差
		int diff = m_viewMaxScore - m_viewScore;
		//差があるなら
		if (diff != 0)
		{
			int absDiff = abs(diff);
			//加算するスコアの基本値
			int speed = kAddBaseSpeed + (absDiff * kAddDiffRate);
			//目標に近いなら少し減速
			if (absDiff < kChangeLowSpeed)
			{
				//誤差埋めの+1
				speed = absDiff / kAddLowSpeedRate + 1;
			}
			if (absDiff <= speed) {
				//ほぼ差がないので一気に目標値へ
				m_viewScore = m_viewMaxScore;
			}
			else
			{
				// スコアをstep分だけ増減
				m_viewScore += speed;
			}
		}
	}
	//跳ねる
	BounceScore(kJumpPower, m_basePos.y);
}

void ScoreUIBase::BounceScore(int jumpPower, int groundY)
{
	//取り出す値
	int scoreValue = m_viewScore;
	for (int i = 0;i < kDigitNum;++i)
	{
		//現在の値
		auto digit = m_digits[i];
		//取り出して保存
		m_digits[i] = scoreValue % 10;
		//桁を下げる
		scoreValue = scoreValue / 10;
		//桁が変わってるなら
		if (digit != m_digits[i])
		{
			//少しはねる
			m_viewVecs[i].y -= jumpPower;
		}
		//これ以上Y座標が下がらないように補正
		m_viewVecs[i].y += kGravity;
		if (m_viewPoses[i].y > groundY)
		{
			m_viewPoses[i].y = groundY;
		}
	}
}