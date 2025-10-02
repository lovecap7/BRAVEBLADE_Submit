#pragma once
#include "BomberStateBase.h"
class GameCamera;
class ActorManager;
class Actor;
class BomberStateIdle :
	public BomberStateBase, public std::enable_shared_from_this<BomberStateIdle>
{
public:
	BomberStateIdle(std::weak_ptr<Actor> owner);
	~BomberStateIdle();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//Œx‰úƒtƒŒ[ƒ€
	int m_warningFrame;
};