#pragma once
#include "BossDragonStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDataLoader.h"
class GameCamera;
class Actor;
class ActorManager;
class Collidable;
class AreaOfEffectAttack;
class BossDragonStatePunchAttack :
	public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStatePunchAttack>
{
public:
	BossDragonStatePunchAttack(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager);
	~BossDragonStatePunchAttack();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//攻撃のフレームを数える
	int m_attackCountFrame;
	//攻撃の参照
	std::weak_ptr<AreaOfEffectAttack> m_attack;
	//攻撃の作成
	void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
	//攻撃の位置を更新する
	void UpdateAttackPos();
	//攻撃データ
	AttackData m_attackData;
};

