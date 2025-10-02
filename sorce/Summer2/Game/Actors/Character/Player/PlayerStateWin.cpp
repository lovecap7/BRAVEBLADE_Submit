#include "PlayerStateWin.h"
#include "Player.h"
#include "../../../../General/game.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Sound/SoundManager.h"
#include "../../../Camera/GameCamera/GameCamera.h"

namespace
{
	//�A�j���[�V����
	const char* kAnim = "Player|Dance";
	//������
	constexpr float kMoveDeceRate = 0.95f;
}

PlayerStateWin::PlayerStateWin(std::weak_ptr<Actor> player) :
	PlayerStateBase(player),
	m_countFrame(0)
{
	//����BGM
	SoundManager::GetInstance().PlayBGM("WinBGM");
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//�������
	coll->GetModel()->SetAnim(kAnim, false);
	coll->SetCollState(CollisionState::Normal);
	//���G
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
	//VC
	coll->CharacterVC("Result");
}

PlayerStateWin::~PlayerStateWin()
{
}

void PlayerStateWin::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}
void PlayerStateWin::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
	++m_countFrame;
	if (m_countFrame <= 1)
	{
		//���f���̌������J����������
		auto dir = camera.lock()->GetPos() - coll->GetPos();
		coll->GetModel()->SetDir(dir.XZ());
	}
}

