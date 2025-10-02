#pragma once
#include "EnemyUIBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>
#include <string>
class EnemyBase;
class EnemyHPUI :
    public EnemyUIBase
{
public:
    EnemyHPUI(std::weak_ptr<EnemyBase> enemy);
    ~EnemyHPUI();
    void Update()override;
    void Draw() const override;
private:
    float m_viewHp;
    float m_viewMaxHp;
    //�`����W
    Vector3 m_pos;
    //�t���[��
	int m_frameHandle;
    //�̗�
	int m_hpHandle;
    //�e�L�X�g
    int m_textHandle;
    //���O
    std::wstring m_name;
};

