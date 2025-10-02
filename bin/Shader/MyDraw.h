#pragma once
#include "../General/Math/MyMath.h"
#include <list>
#include <DxLib.h>
namespace MyDrawUtils
{
	/// <summary>
	/// �傫���Ɗp�x��ύX�ł���`��֐�
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="scale">�傫��</param>
	/// <param name="angle">�p�x</param>
	/// <param name="handle">�摜</param>
	/// /// <param name="texH">�e�N�X�`��</param>
	/// <param name="psH">�s�N�Z���V�F�[�_</param>
	/// <param name="alpha">�����x</param>
	/// <param name="vsH">���_�V�F�[�_</param>
	void DrawRotaGraph(Vector2 pos, float scale, float angle, 
		int handle, int psH, std::list<int> texH = {}, int alpha = 255, int vsH = -1);

	/// <summary>
	/// �`��(shader)
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="handle">�摜</param>
	/// <param name="texH">�e�N�X�`��</param>
	/// <param name="psH">�s�N�Z���V�F�[�_</param>
	void DrawGraph(Vector2 pos, int handle, int psH,std::list<int> texH = {});

	/// <summary>
	/// �~
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="radius"></param>
	/// <param name="segmentCount"></param>
	/// <param name="color"></param>
	void Draw3DCircle(Vector3 pos, float radius, int segmentCount = 36, unsigned int color = GetColor(255, 255, 255));
}