#pragma once
#include "PlayerStateBase.h"
#include "../../../../General/Battle.h"
#include "../../../../General/CSVDataLoader.h"
#include <memory>
class Slash;
class ActorManager;
class MyEffect;
class PlayerStateNA :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateNA>
{
public:
    PlayerStateNA(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager);
    virtual ~PlayerStateNA();
    void Init() override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
protected:
    //攻撃のフレームを数える
    int m_attackCountFrame;
    //攻撃の参照
    std::weak_ptr<Slash> m_attack;
    //攻撃の作成
    void CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager);
    //剣による攻撃の位置を更新する
    void UpdateAttackPos();
    //前進
    void AttackMove(float speed);
    //攻撃データの初期化
    void InitAttackData(const std::weak_ptr<ActorManager> actorManager);
    //攻撃データ
    AttackData m_attackData;
    //攻撃回数
    int m_comboNum;
    //斬撃の参照
    std::weak_ptr<MyEffect> m_eff;
};


