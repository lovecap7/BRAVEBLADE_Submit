#include "MathSub.h"

int MathSub::ClampInt(int value, int min, int max)
{
    //�͈͓��Ɏ��܂��Ă���Ȃ炻�̂܂ܕԂ�
    int clampValue = value;
    //�ŏ���菬�����Ȃ�ŏ��ɍ��킹��
    if (clampValue < min)
    {
        clampValue = min;
    }
    //�ő���傫���Ȃ�ő�ɍ��킹��
    if (clampValue > max)
    {
        clampValue = max;
    }
    return clampValue;
}

float MathSub::ClampFloat(float value, float min, float max)
{
    //�͈͓��Ɏ��܂��Ă���Ȃ炻�̂܂ܕԂ�
    float clampValue = value;
    //�ŏ���菬�����Ȃ�ŏ��ɍ��킹��
    if (clampValue < min)
    {
        clampValue = min;
    }
    //�ő���傫���Ȃ�ő�ɍ��킹��
    if (clampValue > max)
    {
        clampValue = max;
    }
    return clampValue;
}

float MathSub::Lerp(float start, float end, float t)
{
    float result = start * (1.0f - t) + end * t;
    return result;
}

float MathSub::Min(float a, float b)
{
    float min = a;
    if (a > b)
    {
        min = b;
    }
    return min;
}

float MathSub::Max(float a, float b)
{
    float max = a;
    if (a < b)
    {
        max = b;
    }
    return max;
}

float MathSub::Sign(float num)
{
    if (num == 0.0f)
    {
        return 0.0f;
    }
    else if (num > 0.0f)
    {
        return 1.0f;
    }
    else
    {
        return -1.0f;
    }
}
