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
    //描画座標
    Vector3 m_pos;
    //フレーム
	int m_frameHandle;
    //体力
	int m_hpHandle;
    //テキスト
    int m_textHandle;
    //名前
    std::wstring m_name;
};

