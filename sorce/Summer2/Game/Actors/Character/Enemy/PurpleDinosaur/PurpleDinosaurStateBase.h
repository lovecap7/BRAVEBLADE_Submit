#pragma once
#include "../../CharacterStateBase.h"
#include <memory>
#include <vector>
#include <string>
#include "../../../../../General/Math/Vector3.h"
class Actor;
class GameCamera;
class ActorManager;
class PurpleDinosaurStateBase abstract :
	public CharacterStateBase
{
public:
	PurpleDinosaurStateBase(std::weak_ptr<Actor> owner);
	virtual ~PurpleDinosaurStateBase();
};
//攻撃のデータを検索するときに使う
namespace
{
	std::string kOwnerName = "PurpleDinosaur";
	std::string kAttackName = "Punch";
}

