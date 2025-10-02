#pragma once
#include "BossKingStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDataLoader.h"
#include "../../../../../General/Math/MyMath.h"

class GameCamera;
class Input;
class ActorManager;
class Actor;
class BossKingStateImpact :
	public BossKingStateBase, public std::enable_shared_from_this<BossKingStateImpact>
{
public:
	BossKingStateImpact(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager);
	~BossKingStateImpact();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//�U���̃t���[���𐔂���
	int m_attackCountFrame;
	//�U���̍쐬
	void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
	//�U���f�[�^
	AttackData m_attackData;
};

