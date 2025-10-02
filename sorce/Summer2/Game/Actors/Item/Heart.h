#pragma once
#include "ItemBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class GameCamera;
class Heart :
	public ItemBase
{
public:
	Heart(int modelHandle, Vector3 pos);
	virtual ~Heart();
	//Õ“ËƒCƒxƒ“ƒg
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//€–Sˆ—
	void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override;
private:

};

