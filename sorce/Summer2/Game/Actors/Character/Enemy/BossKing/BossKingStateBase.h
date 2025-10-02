#pragma once
#include "../../CharacterStateBase.h"
#include "../../CharacterBase.h"
#include <memory>
#include <vector>
#include <string>
#include "../../../../../General/Math/Vector3.h"

namespace
{
	//�ߋ���
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
	//���`��
	bool m_isTransformSecond;
	//�U����i���l����
	void ThinkAttack(const std::weak_ptr<ActorManager> actorManager);
	//�����_���U���{�C�X
	void RandAttackVC();
private:
	//���`�Ԃ̍U����i
	void ThinkAttackFirst(Actor::TargetData& targetData, const std::weak_ptr<ActorManager> actorManager);
	//���`�Ԃ̍U����i
	void ThinkAttackSecond(Actor::TargetData& targetData, const std::weak_ptr<ActorManager> actorManager);
};

//�U���̃f�[�^����������Ƃ��Ɏg��
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


