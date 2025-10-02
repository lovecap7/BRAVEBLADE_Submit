#pragma once
#include "BossKingStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDataLoader.h"
#include "../../../../../General/Math/MyMath.h"

class GameCamera;
class Input;
class ActorManager;
class Actor;
class MyEffect;
class BossKingStateBlastAttack :
	public BossKingStateBase, public std::enable_shared_from_this<BossKingStateBlastAttack>
{
public:
	BossKingStateBlastAttack(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager);
	~BossKingStateBlastAttack();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//攻撃のフレームを数える
	int m_attackCountFrame;
	//攻撃の作成
	void CreateAttack(const std::weak_ptr<ActorManager> actorManager, Vector3 pos);
	//攻撃データ
	AttackData m_attackData;
	//魔法陣エフェクト
	std::weak_ptr<MyEffect> m_blastMagicEff;

private:
	//魔法陣の位置更新
	void UpdateBlastMagicEffPos(Vector3 pos);
};


