#include "PlayerStateHit.h"
#include "PlayerStateIdle.h"
#include "PlayerStateDeath.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "../Enemy/EnemyBase.h"
#include "../../ActorManager.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/GameCamera/GameCamera.h"
#include <DxLib.h>
namespace
{
	//�A�j���[�V����
	const char* kHit1Anim = "Player|Hit1";
	const char* kHit2Anim = "Player|Hit2";
	//�A�j���[�V�������x
	constexpr float kHit1AnimSpeed = 1.1f;
	constexpr float kHit2AnimSpeed = 1.1f;
	//������
	constexpr float kMoveDeceRate = 0.95f;
}


PlayerStateHit::PlayerStateHit(std::weak_ptr<Actor> player) :
	PlayerStateBase(player)
{
	//�����_���Ƀq�b�g�A�j���[�V������I��
	RandHitAnim();
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//�����_���_���[�WVC
	RandDamageVC(coll);
}


PlayerStateHit::~PlayerStateHit()
{
}

void PlayerStateHit::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateHit::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//�{�X�����S�ɏ��ł����Ƃ�
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
	//���S����
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_owner));
		return;
	}
	//�U�����󂯂��Ȃ�
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		//�ŏ�����Đ�
		coll->GetModel()->ReplayAnim();
	}
	
	//���񂾂񌸑�
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void PlayerStateHit::RandHitAnim()
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//�����_���Ńq�b�g�A�j���[�V������I��
	if (GetRand(1))//0��1�Ń����_��
	{
		coll->GetModel()->SetAnim(kHit1Anim, false, kHit1AnimSpeed);
	}
	else
	{
		coll->GetModel()->SetAnim(kHit2Anim, false, kHit2AnimSpeed);
	}
}
void PlayerStateHit::RandDamageVC(std::shared_ptr<Player> coll)
{
	switch (MyMath::GetRand(1, 3))
	{
	case 1:
		coll->CharacterVC("Damage1");
		break;
	case 2:
		coll->CharacterVC("Damage2");
		break;
	case 3:
		coll->CharacterVC("Damage3");
		break;
	default:
		break;
	}
}