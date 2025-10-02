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
    //フレーム
	int m_frameHandle;
	//体力
	int m_hpHandle;
	//テキスト
	int m_textHandle;
    //名前
	std::wstring m_name;
};

