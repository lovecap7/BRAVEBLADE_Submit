#pragma once
#include "SphereAttackBase.h"
class Actor;
class CharacterStateBase;
class HomingBullet :
	public SphereAttackBase
{
public:
	HomingBullet(std::weak_ptr<Actor> owner);
	virtual ~HomingBullet() {};
	//更新処理
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//衝突イベント
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//描画
	void Draw() const override;
	//移動量
	void SetVec(Vector3 vec);
	//ターゲット
	void SetTarget(std::weak_ptr<Actor> target);
	//速度
	void SetSpeed(float speed) { m_speed = speed; };
	//追跡力
	void SetTrackingRate(float trackingRate) { m_trackingRate = trackingRate; };
private:
	std::weak_ptr<Actor> m_target;
	//速度
	float m_speed;
	//吸い付き
	float m_trackingRate;
};

