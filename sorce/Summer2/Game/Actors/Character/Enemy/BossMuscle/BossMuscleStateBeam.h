#pragma once
#include "BossMuscleStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDataLoader.h"

class GameCamera;
class Collidable;
class Model;
class Input;
class ActorManager;
class Actor;
class MyEffect;
class ULT;
class BossMuscle;
class BossMuscleStateBeam :
	public BossMuscleStateBase, public std::enable_shared_from_this<BossMuscleStateBeam>
{
public:
	BossMuscleStateBeam(std::weak_ptr<Actor> owner, bool isAngry, const std::weak_ptr<ActorManager> actorManager);
	~BossMuscleStateBeam();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//攻撃のフレームを数える
	int m_attackCountFrame;
	//攻撃位置の更新
	void UpdateBeamPos(const Vector3 pos, std::shared_ptr<Model> model, std::shared_ptr<BossMuscle> coll);
    //攻撃の作成
    void CreateAttack(VECTOR pos,const std::weak_ptr<ActorManager> actorManager);
    //攻撃データ
    AttackData m_attackData;
	//ビームのチャージエフェクト参照
	std::weak_ptr<MyEffect> m_beamChargeEff;
	//ビームのエフェクト参照
	std::weak_ptr<MyEffect> m_beamEff;
	//ビームの攻撃参照
	std::weak_ptr<ULT> m_beam;
};