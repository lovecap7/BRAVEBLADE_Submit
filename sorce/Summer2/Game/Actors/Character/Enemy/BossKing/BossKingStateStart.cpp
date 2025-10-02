#include "BossKingStateStart.h"
#include "BossKingStateIdle.h"
#include "BossKing.h"
#include "../EnemyBase.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../ActorManager.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../../General/Sound/SoundManager.h"
namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V����
	const char* kAnim = "Boss3|Start";
}

BossKingStateStart::BossKingStateStart(std::weak_ptr<Actor> owner) :
	BossKingStateBase(owner,false)
{
	//�X�^�[�g���
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, false);
	coll->SetCollState(CollisionState::Normal);
	//�I�[���G�t�F�N�g
	EffekseerManager::GetInstance().CreateTrackActorEffect("BossKingStartEff", m_owner);
}

BossKingStateStart::~BossKingStateStart()
{
	
}

void BossKingStateStart::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void BossKingStateStart::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//���͂��~�߂�
	auto& input = Input::GetInstance();
	input.StopUpdate();
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	//���f���̃A�j���[�V�������I�������
	if (coll->GetModel()->IsFinishAnim())
	{
		//���͂��J�n
		input.StartUpdate();
		//�ҋ@
		ChangeState(std::make_shared<BossKingStateIdle>(m_owner,false));
		return;
	}
	//�Z���t�ƕ�
	if (coll->GetModel()->GetNowAnimFrame() == coll->GetModel()->GetTotalAnimFrame() * 0.25f)
	{
		//VC
		coll->CharacterVC("Start");
		//SE
		SoundManager::GetInstance().PlayOnceSE("Wind");
	}
	//����
	else if (coll->GetModel()->GetNowAnimFrame() == coll->GetModel()->GetTotalAnimFrame() * 0.5f)
	{
		//SE
		SoundManager::GetInstance().PlayOnceSE("Blast");
	}

	//���񂾂񌸑�
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
