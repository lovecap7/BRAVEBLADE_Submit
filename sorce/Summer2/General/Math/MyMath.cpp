#include "MyMath.h"
#include <random>
#include <cmath>
float MyMath::GetRandF(float min, float max)
{
    // �񌈒�I�ȗ���������
    static std::random_device rd;
    // �����Z���k�E�c�C�X�^�@�ɂ��[������������
    static std::mt19937 mt(rd());
    // min����max�͈̔͂ň�l���z�̗����𐶐�
    std::uniform_real_distribution<float> dist(min, max);
    return dist(mt);
}

int MyMath::GetRand(int min, int max)
{
    // �񌈒�I�ȗ���������
    static std::random_device rd;
    // �����Z���k�E�c�C�X�^�@�ɂ��[������������
    static std::mt19937 mt(rd());
    // min����max�͈̔͂ň�l���z�̗����𐶐�
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}

bool MyMath::IsRand()
{
    return GetRand(0, 1);
}
