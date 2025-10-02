#pragma once
#include "PlayerStateBase.h"
class Actor;
class Input;
class GameCamera;
class ActorManager;
class Player;
class PlayerStateHit :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateHit>
{
public:
    PlayerStateHit(std::weak_ptr<Actor> player);
    ~PlayerStateHit();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//ランダムでヒットアニメーションを選ぶ
	void RandHitAnim();
    //ランダムでダメージVC
    void RandDamageVC(std::shared_ptr<Player> coll);
};

