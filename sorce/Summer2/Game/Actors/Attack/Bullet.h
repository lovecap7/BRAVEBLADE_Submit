#pragma once
#include "SphereAttackBase.h"
class Actor;
class CharacterStateBase;
class Bullet:
	public SphereAttackBase
{
public:
	Bullet(std::weak_ptr<Actor> owner);
	virtual ~Bullet() {};
    //更新処理
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//衝突イベント
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//描画
	void Draw() const override;
	//移動量
	void SetVec(Vector3 vec);
};

