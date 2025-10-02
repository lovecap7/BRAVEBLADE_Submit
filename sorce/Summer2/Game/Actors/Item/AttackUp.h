#pragma once
#include "ItemBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class GameCamera;
class AttackUp :
	public ItemBase
{
public:
	AttackUp(int modelHandle, Vector3 pos);
	virtual ~AttackUp();
	//�Փ˃C�x���g
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//���S����
	void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override;
private:

};

