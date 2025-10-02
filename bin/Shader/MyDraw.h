#pragma once
#include "../General/Math/MyMath.h"
#include <list>
#include <DxLib.h>
namespace MyDrawUtils
{
	/// <summary>
	/// 大きさと角度を変更できる描画関数
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="scale">大きさ</param>
	/// <param name="angle">角度</param>
	/// <param name="handle">画像</param>
	/// /// <param name="texH">テクスチャ</param>
	/// <param name="psH">ピクセルシェーダ</param>
	/// <param name="alpha">透明度</param>
	/// <param name="vsH">頂点シェーダ</param>
	void DrawRotaGraph(Vector2 pos, float scale, float angle, 
		int handle, int psH, std::list<int> texH = {}, int alpha = 255, int vsH = -1);

	/// <summary>
	/// 描画(shader)
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="handle">画像</param>
	/// <param name="texH">テクスチャ</param>
	/// <param name="psH">ピクセルシェーダ</param>
	void DrawGraph(Vector2 pos, int handle, int psH,std::list<int> texH = {});

	/// <summary>
	/// 円
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="radius"></param>
	/// <param name="segmentCount"></param>
	/// <param name="color"></param>
	void Draw3DCircle(Vector3 pos, float radius, int segmentCount = 36, unsigned int color = GetColor(255, 255, 255));
}