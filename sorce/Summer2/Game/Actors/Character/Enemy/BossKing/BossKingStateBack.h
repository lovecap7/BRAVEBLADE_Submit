#pragma once
#include "BossKingStateBase.h"
class BossKingStateBack :
	public BossKingStateBase, public std::enable_shared_from_this<BossKingStateBack>
{
public:
	BossKingStateBack(std::weak_ptr<Actor> owner);
	~BossKingStateBack();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//ˆÚ“®ƒtƒŒ[ƒ€
	int m_moveFrame;
};

