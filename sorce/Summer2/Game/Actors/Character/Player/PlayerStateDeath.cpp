#include "PlayerStateDeath.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "../../../../General/game.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../Camera/GameCamera/GameCamera.h"

namespace
{
	//�A�j���[�V����
	const char* kAnim = "Player|Dead";
	//������
	constexpr float kMoveDeceRate = 0.95f;
}

PlayerStateDeath::PlayerStateDeath(std::weak_ptr<Actor> player) :
	PlayerStateBase(player)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//���S���
	coll->GetModel()->SetAnim(kAnim, false);
	coll->SetCollState(CollisionState::Dead);
	//���G
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
	//VC
	coll->CharacterVC("Dead");
}

PlayerStateDeath::~PlayerStateDeath()
{
}

void PlayerStateDeath::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}
void PlayerStateDeath::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//�������̗͂��񕜂�����ҋ@��Ԃɖ߂�
	if (coll->GetHitPoints().expired())return;
	if (!coll->GetHitPoints().lock()->IsDead())
	{
		//���G����
		coll->GetHitPoints().lock()->SetIsNoDamege(false);
		//�����G�t�F�N�g
		EffekseerManager::GetInstance().CreateTrackActorEffect("RevivalPlayerEff", coll);
		//�ҋ@��Ԃɖ߂�
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//�A�j���[�V�����I����
	if (coll->GetModel()->IsFinishAnim())
	{
		//���S
		coll->SetIsDead(true);
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

