#include "PlayerStateRolling.h"
#include "PlayerStateIdle.h"
#include "PlayerStateWin.h"
#include "PlayerStateHit.h"
#include "Player.h"
#include "../Enemy/EnemyBase.h"
#include "../../../../General/game.h"
#include "../../ActorManager.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Effect/TrackActorEffect.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../Game/Camera/GameCamera/GameCamera.h"

namespace
{
	//��𑬓x
	constexpr float kRollingMoveSpeed = 10.0f;
	//�A�j���[�V����
	const char* kAnim = "Player|Rolling";
	//������[�V�����̑��x
	constexpr float kAnimSpeed = 0.8f;
	//�����Ԃ̃t���[����
	constexpr int kRollingFrame = 16;
}

PlayerStateRolling::PlayerStateRolling(std::weak_ptr<Actor> player) :
	PlayerStateBase(player)
{
	//������
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	coll->SetCollState(CollisionState::Normal);
	//�����̍X�V
	coll->GetModel()->SetDir(coll->GetPlayerStickVec());
	//���G
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
	//�����_��VC
	RandRollingVC(coll);
}

PlayerStateRolling::~PlayerStateRolling()
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//���G����
	coll->GetHitPoints().lock()->SetIsNoDamege(false);
}
void PlayerStateRolling::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
	//�G�t�F�N�g
	EffekseerManager::GetInstance().CreateTrackActorEffect("RollingEff", m_owner);
	EffekseerManager::GetInstance().CreateEffect("LowShockWaveEff", m_owner.lock()->GetPos());
}

void PlayerStateRolling::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//�{�X��|��
	if (actorManager.lock()->IsBossDisappear())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//�{�X�̗̑͂��Ȃ��Ȃ����ꍇ�܂��̓��f���̃A�j���[�V�������I�������
	if (actorManager.lock()->IsBossDead() ||
		coll->GetModel()->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//�U�����󂯂��Ȃ�
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_owner));
		return;
	}
	//���G�t���[���I�������
	if(coll->GetModel()->GetNowAnimFrame() >= kRollingFrame)
	{
		coll->GetHitPoints().lock()->SetIsNoDamege(false);
	}

	auto rb = coll->GetRb();
	//�����Ă�����Ɉړ�
	rb->SetMoveVec(coll->GetModel()->GetDir() * kRollingMoveSpeed);
}

void PlayerStateRolling::RandRollingVC(std::shared_ptr<Player> coll)
{
	if (MyMath::IsRand())
	{
		coll->CharacterVC("Rolling1");
	}
	else
	{
		coll->CharacterVC("Rolling2");
	}
}