#pragma once
#include "Math/Vector3.h"
//基本16:9 (4:3もOK)
//このサイズ以外は特別な理由がない限り基本ダメ
namespace Game//属性
{
	//画面の大きさを定数定義	16:9
	constexpr int kScreenWidth = 1280;
	constexpr int kScreenHeight = 720;
	constexpr int kColorBitNum = 32;
	constexpr int kScreenCenterX = kScreenWidth / 2;
	constexpr int kScreenCenterY = kScreenHeight / 2;

	constexpr bool kDefaultWindowMode = true;

	//プレイヤーのコントローラーインデックス
	enum class PlayerIndex
	{
		None,
		Player1,
		Player2,
		Player3,
		Player4,
	};
}

namespace Gravity
{
	//重力
	const Vector3 kGroundGravity = { 0.0f, -5.0f, 0.0f };
	//空中にいるオブジェクトに対する重力
	const Vector3 kAirGravity = { 0.0f, -0.5f, 0.0f };
	//落下状態に切り替わる落下ベクトルの大きさ
	constexpr float kChangeStateFall = -4.0f;
	//落下スピードの上限(Y成分の大きさ)
	constexpr float kMaxGravityY = -20.0f;
}