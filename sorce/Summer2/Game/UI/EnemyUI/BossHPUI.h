#pragma once
#include "EnemyUIBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>
#include <string>
class EnemyBase;
class BossHPUI :
    public EnemyUIBase
{
public:
    BossHPUI(std::weak_ptr<EnemyBase> enemy);
    ~BossHPUI();
    void Update()override;
    void Draw() const override;
private:
    float m_viewHp;
    float m_viewMaxHp;
    //�t���[��
	int m_frameHandle;
	//�̗�
	int m_hpHandle;
	//�e�L�X�g
	int m_textHandle;
    //���O
	std::wstring m_name;
};

