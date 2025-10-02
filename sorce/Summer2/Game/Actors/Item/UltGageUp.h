#pragma once
#include "ItemBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class GameCamera;
class UltGageUp :
	public ItemBase
{
public:
	UltGageUp(int modelHandle, Vector3 pos);
	virtual ~UltGageUp();
	//Õ“ËƒCƒxƒ“ƒg
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//€–Sˆ—
	void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override;
private:

};

