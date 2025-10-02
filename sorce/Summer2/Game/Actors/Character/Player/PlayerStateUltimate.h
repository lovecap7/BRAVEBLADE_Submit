#pragma once
#include "PlayerStateBase.h"
#include "../../../../General/Battle.h"
#include "../../../../General/CSVDataLoader.h"
class Actor;
class ULT;
class GameCamera;
class ActorManager;
class MyEffect;
class PlayerStateUltimate :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateUltimate>
{
public:
    PlayerStateUltimate(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager);
    ~PlayerStateUltimate();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //アニメーションのフレーム
    int m_animCountFrame;
    //アニメーションの速度
    float m_animSpeed;
    //攻撃の参照
    std::weak_ptr<ULT> m_attack;
    //攻撃の作成
    void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
	//攻撃の位置更新
    void UpdateLaserPos();
    //攻撃データ
    AttackData m_attackData;
    //剣のエフェクト参照
    std::weak_ptr<MyEffect> m_laserChargeEff;
    std::weak_ptr<MyEffect> m_laserEff;
};


