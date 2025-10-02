#pragma once
#include "PlayerStateBase.h"
#include "../../../../General/Battle.h"
#include "../../../../General/CSVDataLoader.h"
class Actor;
class Slash;
class TrackActorEffect;
class MyEffect;
class PlayerStateCA :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateCA>
{
public:
    PlayerStateCA(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager,AttackData data);
    virtual ~PlayerStateCA();
    void Init() override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
protected:
    //攻撃のフレームを数える
    int m_attackCountFrame;
    //攻撃の参照
    std::weak_ptr<Slash> m_attack;
    //攻撃の作成
    void CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager);
    //足による攻撃の位置を更新する
    void UpdateAttackPos();
    //攻撃データ
    AttackData m_attackData;
    //エフェクトの参照
    std::weak_ptr<TrackActorEffect> m_tornadeEff;
    std::weak_ptr<MyEffect> m_slashEff;
    //回転量
    float m_attackRotaAngle;
    //1回転にかかるフレーム
    int m_oneRotaFrame;
};

