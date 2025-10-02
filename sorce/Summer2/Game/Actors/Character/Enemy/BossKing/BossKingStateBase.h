#pragma once
#include "../../CharacterStateBase.h"
#include "../../CharacterBase.h"
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
class BossKingStateBase abstract :
	public CharacterStateBase
{
public:
	BossKingStateBase(std::weak_ptr<Actor> owner, bool isTransformSecond);
	virtual ~BossKingStateBase();
protected:
	//第二形態
	bool m_isTransformSecond;
	//攻撃手段を考える
	void ThinkAttack(const std::weak_ptr<ActorManager> actorManager);
	//ランダム攻撃ボイス
	void RandAttackVC();
private:
	//第一形態の攻撃手段
	void ThinkAttackFirst(Actor::TargetData& targetData, const std::weak_ptr<ActorManager> actorManager);
	//第二形態の攻撃手段
	void ThinkAttackSecond(Actor::TargetData& targetData, const std::weak_ptr<ActorManager> actorManager);
};

//攻撃のデータを検索するときに使う
namespace
{
	std::string kOwnerName			= "BossKing";
	std::string kMagicWaveName		= "MagicWave";
	std::string kBlastAttackName	= "BlastAttack";
	std::string kBeamName			= "Beam";
	std::string kRapidFireName		= "RapidFire";
	std::string kImpactName			= "Impact";
	std::string kUltName			= "Ult";
}


