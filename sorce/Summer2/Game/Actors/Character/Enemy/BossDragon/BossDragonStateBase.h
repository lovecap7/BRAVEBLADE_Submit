#pragma once
#include "../../CharacterStateBase.h"
#include <memory>
#include <vector>
#include <string>
#include "../../../../../General/Math/Vector3.h"
class Actor;
class GameCamera;
class ActorManager;
class BossDragonStateBase abstract :
	public CharacterStateBase
{
public:
	BossDragonStateBase(std::weak_ptr<Actor> owner);
	virtual ~BossDragonStateBase();
protected:
	//攻撃を選ぶ
	void ThinkAttack(const std::weak_ptr<ActorManager> actorManager);
};
//攻撃のデータを検索するときに使う
namespace
{
	std::string kOwnerName = "BossDragon";
	std::string kBreathName = "Breath";
	std::string kPunchName = "Punch";
	std::string kSweepName = "Sweep";
}


