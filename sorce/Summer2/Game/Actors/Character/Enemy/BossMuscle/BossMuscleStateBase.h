#pragma once
#include "../../CharacterStateBase.h"
#include <memory>
#include <vector>
#include <string>
#include "../../../../../General/Math/Vector3.h"

namespace
{
	//近距離
	constexpr float kMeleeAttackDistance = 700.0f;
}

class Actor;
class ActorManager;
class BossMuscleStateBase abstract :
	public CharacterStateBase
{
public:
	BossMuscleStateBase(std::weak_ptr<Actor> owner, bool isAngry);
	virtual ~BossMuscleStateBase();
protected:
	bool m_isAngry; //怒り状態かどうか
	//攻撃手段を考える
	void ThinkAttack(const std::weak_ptr<ActorManager> actorManager);
};
//攻撃のデータを検索するときに使う
namespace
{
	std::string kOwnerName			= "BossMuscle";
	std::string kRightPunchName		= "RightPunch";
	std::string kBeamName			= "Beam";
	std::string kJumpAttackName		= "JumpAttack";
	std::string kSwingAttackName	= "SwingAttack";
}


