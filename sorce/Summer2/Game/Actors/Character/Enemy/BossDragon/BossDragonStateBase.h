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
	//�U����I��
	void ThinkAttack(const std::weak_ptr<ActorManager> actorManager);
};
//�U���̃f�[�^����������Ƃ��Ɏg��
namespace
{
	std::string kOwnerName = "BossDragon";
	std::string kBreathName = "Breath";
	std::string kPunchName = "Punch";
	std::string kSweepName = "Sweep";
}


