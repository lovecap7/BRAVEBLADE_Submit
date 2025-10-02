#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "MathSub.h"

namespace MyMath
{
	constexpr double PI = 3.14159265358979323846;
	constexpr double TwoPI = 3.14159265358979323846 * 2.0;
	constexpr float PI_F = 3.14159265358979323846f;
	constexpr float TwoPI_F = 3.14159265358979323846f * 2.0f;


	// �x���@���ʓx�@�ɕύX
	constexpr float DEG_2_RAD = PI_F / 180.0f;
	// �ʓx�@��x���@�ɕύX
	constexpr float RAD_2_DEG = 180.0f / PI_F;

	/// <summary>
	/// �����_���Ȓl�𐶐����܂��B
	/// min�ȏ�max�����̒l��Ԃ��܂��B
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns>float�^</returns>
	float GetRandF(float min, float max);
	/// <summary>
	/// �����_���Ȓl�𐶐����܂��B
	/// min�ȏ�max�ȉ��̒l��Ԃ��܂��B
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns>int�^</returns>
	int GetRand(int min, int max);
	/// <summary>
	/// �����_���ŃI���I�t��Ԃ�
	/// </summary>
	/// <returns></returns>
	bool IsRand();
}

