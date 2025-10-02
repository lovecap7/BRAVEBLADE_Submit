#pragma once
#include "BossMuscleStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDataLoader.h"

class GameCamera;
class AreaOfEffectAttack;
class Input;
class ActorManager;
class Actor;
class BossMuscleStateSwingAttack :
	public BossMuscleStateBase, public std::enable_shared_from_this<BossMuscleStateSwingAttack>
{
public:
	BossMuscleStateSwingAttack(std::weak_ptr<Actor> owner, bool isAngry, const std::weak_ptr<ActorManager> actorManager);
	~BossMuscleStateSwingAttack();
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
	//攻撃時に前進する
	void AttackMove();
	//攻撃データ
	AttackData m_attackData;
};