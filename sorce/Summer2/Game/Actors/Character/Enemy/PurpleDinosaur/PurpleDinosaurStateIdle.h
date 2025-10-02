#pragma once
#include "PurpleDinosaurStateBase.h"
class GameCamera;
class ActorManager;
class Actor;
class PurpleDinosaurStateIdle :
	public PurpleDinosaurStateBase, public std::enable_shared_from_this<PurpleDinosaurStateIdle>
{
public:
	PurpleDinosaurStateIdle(std::weak_ptr<Actor> owner);
	~PurpleDinosaurStateIdle();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//Œx‰úƒtƒŒ[ƒ€
	int m_warningFrame;
};