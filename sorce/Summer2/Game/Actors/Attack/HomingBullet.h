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
	//�X�V����
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//�Փ˃C�x���g
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//�`��
	void Draw() const override;
	//�ړ���
	void SetVec(Vector3 vec);
	//�^�[�Q�b�g
	void SetTarget(std::weak_ptr<Actor> target);
	//���x
	void SetSpeed(float speed) { m_speed = speed; };
	//�ǐ՗�
	void SetTrackingRate(float trackingRate) { m_trackingRate = trackingRate; };
private:
	std::weak_ptr<Actor> m_target;
	//���x
	float m_speed;
	//�z���t��
	float m_trackingRate;
};

