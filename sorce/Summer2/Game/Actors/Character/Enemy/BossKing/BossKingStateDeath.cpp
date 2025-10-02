#include "BossKingStateDeath.h"
#include "BossKing.h"
#include "../EnemyBase.h"
#include "../../../Actor.h"
#include "../../../ActorManager.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../../../General/Sound/SoundManager.h"
#include "../../../../../General/Effect/EffekseerManager.h"

namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V����
	const char* kAnim = "Boss3|Boss3|Boss3|Dead";
	//��������
	constexpr float kModelHeightAdjust = 1.0f;
}

BossKingStateDeath::BossKingStateDeath(std::weak_ptr<Actor> owner) :
	BossKingStateBase(owner,true)
{
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	//���S��Ԃɂ���
	coll->SetCollState(CollisionState::Dead);
	//���S
	coll->GetModel()->SetAnim(kAnim, false);
	//���G
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
	//BGM���~�߂�
	SoundManager::GetInstance().StopBGM();
	//�G�t�F�N�g�̍폜
	EffekseerManager::GetInstance().Reset();
	//��������
	coll->GetModel()->SetModelHeightAdjust(kModelHeightAdjust);
	//VC
	coll->CharacterVC("Dead");
}

BossKingStateDeath::~BossKingStateDeath()
{
}

void BossKingStateDeath::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossKingStateDeath::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	//�A�j���[�V�����I����
	if (coll->GetModel()->IsFinishAnim())
	{
		coll->Delete();//�폜
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
