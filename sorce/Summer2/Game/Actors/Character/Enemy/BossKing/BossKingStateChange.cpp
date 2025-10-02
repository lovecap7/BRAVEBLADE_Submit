#include "BossKingStateChange.h"
#include "BossKingStateIdle.h"
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
#include "../../../../UI/UIManager.h"

namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V����
	const char* kDeathAnim = "Boss3|Death";
	const char* kStandingAnim = "Boss3|Standing";
	//�A�j���[�V�������x
	constexpr float kAnimSpeed = 0.3f;
}

BossKingStateChange::BossKingStateChange(std::weak_ptr<Actor> owner) :
	BossKingStateBase(owner,false)
{
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//���S
	coll->GetModel()->SetAnim(kDeathAnim, false, kAnimSpeed);
	//���G
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
	//VC
	coll->CharacterVC("Change");
	//SE
	coll->CharacterOnceSE("Change");
}

BossKingStateChange::~BossKingStateChange()
{
}

void BossKingStateChange::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
	//�G�t�F�N�g
	EffekseerManager::GetInstance().CreateTrackActorEffect("BossKingTransEff", m_owner);
}

void BossKingStateChange::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//���͂��~�߂�
	auto& input = Input::GetInstance();
	input.StopUpdate();
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	//�A�j���[�V�����I����
	if (coll->GetModel()->IsFinishAnim())
	{
		if (m_isTransformSecond)
		{
			//�J�n
			input.StartUpdate();
			//UI�̕`����J�n
			UIManager::GetInstance().StartDraw();
			//�S��
			coll->FullRecovery();
			//���G����
			coll->GetHitPoints().lock()->SetIsNoDamege(false);
			//�ҋ@���
			ChangeState(std::make_shared<BossKingStateIdle>(m_owner, true));
			return;
		}
		else
		{
			//SE
			coll->CharacterOnceSE("Thunder");
			//�ϐg
			coll->TransformSecond();
			//�����オ��
			coll->GetModel()->SetAnim(kStandingAnim, false);
			//�ϐg�t���O
			m_isTransformSecond = true;
		}
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
