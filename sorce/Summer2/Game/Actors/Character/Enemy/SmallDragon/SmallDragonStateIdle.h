#pragma once
#include "SmallDragonStateBase.h"
class Actor;
class GameCamera;
class ActorManager;
class SmallDragonStateIdle :
    public SmallDragonStateBase, public std::enable_shared_from_this<SmallDragonStateIdle>
{
public:
	SmallDragonStateIdle(std::weak_ptr<Actor> owner);
	~SmallDragonStateIdle();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//Œx‰úƒtƒŒ[ƒ€
	int m_warningFrame;
};