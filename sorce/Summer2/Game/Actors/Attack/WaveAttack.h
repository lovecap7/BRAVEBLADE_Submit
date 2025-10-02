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
    //�g�̍L���鑬�x��ݒ�
    void SetWaveSpeed(float speed) { m_waveSpeed = speed; }
private:
    //�g�̍L���鑬�x
	float m_waveSpeed;
};

