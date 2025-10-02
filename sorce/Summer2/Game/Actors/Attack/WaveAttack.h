#pragma once
#include "TorusAttackBase.h"
class WaveAttack :
    public TorusAttackBase
{
public:
    WaveAttack(std::weak_ptr<Actor> owner);
    ~WaveAttack() override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    void Draw() const override;
	void OnCollide(const std::shared_ptr<Collidable> other) override;
    //”g‚ÌL‚ª‚é‘¬“x‚ğİ’è
    void SetWaveSpeed(float speed) { m_waveSpeed = speed; }
private:
    //”g‚ÌL‚ª‚é‘¬“x
	float m_waveSpeed;
};

