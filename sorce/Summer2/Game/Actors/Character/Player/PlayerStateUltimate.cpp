#include "PlayerStateUltimate.h"
#include "PlayerStateIdle.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "UltGage.h"
#include "../../ActorManager.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/SphereCollider.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../General/Effect/MyEffect.h"
#include "../../Attack/ULT.h"
#include "../../../../Game/Camera/GameCamera/GameCamera.h"

namespace
{
	//����̍��W
	constexpr int kRightHandIndex = 43;
	//����̒���
	constexpr float kSwordHeight = 3000.0f;
	//�U����������Z�b�g����p�x
	constexpr int kUltResetFrame = 10;
	//�A�j���[�V�����̑��x�̕ω���
	constexpr float kAddAnimSpeed = 0.2f;
	//�A�j���[�V�����̍ō����x
	constexpr float kMaxAnimSpeed = 2.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//���f���̐��񑬓x
	constexpr int kModelRotateSpeed = 100;
}

PlayerStateUltimate::PlayerStateUltimate(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager) :
	PlayerStateBase(player),
	m_animCountFrame(0)
{
	//�`���[�W�t���[���������t���[�����傫�������r
	m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kULTName);
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//�K�E�Z
	coll->SetCollState(CollisionState::Normal);
	auto model = coll->GetModel();
	m_animSpeed = m_attackData.animSpeed;
	model->SetAnim(m_attackData.anim.c_str(), false, m_animSpeed);
	//�����̍X�V
	Vector2 dir = coll->GetPlayerStickVec();
	model->SetDir(dir);
	//�Q�[�W��0��
	coll->GetUltGage().lock()->ResetUltGage();
	coll->GetUltGage().lock()->SetPendingUltGage(0);
	//���G
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
	//���̃G�t�F�N�g
	m_laserChargeEff = EffekseerManager::GetInstance().CreateEffect("UltChargeSwordEff", m_owner.lock()->GetPos());
	//���f���̐��񑬓x
	model->SetRotSpeed(kModelRotateSpeed);
	//SE
	coll->CharacterOnceSE("UltCharge");
	//VC
	coll->CharacterVC("Ult");
}


PlayerStateUltimate::~PlayerStateUltimate()
{
	//�U������̍폜
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	if (!m_attack.expired())m_attack.lock()->Delete();
	//�G�t�F�N�g�̍폜
	if (!m_laserChargeEff.expired())m_laserChargeEff.lock()->Delete();
	if (!m_laserEff.expired())m_laserEff.lock()->Delete();
	//���G
	coll->GetHitPoints().lock()->SetIsNoDamege(false);
}

void PlayerStateUltimate::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateUltimate::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	++m_animCountFrame;
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto model = coll->GetModel();
	//�{�X��|��
	if (actorManager.lock()->IsBossDisappear())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//�{�X�̗̑͂��Ȃ��Ȃ����ꍇ�܂��̓A�j���[�V�������I��������
	if (actorManager.lock()->IsBossDead() ||
		model->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//�`���[�W�G�t�F�N�g�̈ʒu�X�V
	if (!m_laserChargeEff.expired())
	{
		VECTOR rightHand = MV1GetFramePosition(model->GetModelHandle(), kRightHandIndex);
		m_laserChargeEff.lock()->SetPos(rightHand);
	}

	//�U�������t���[��
	if (m_animCountFrame == m_attackData.startFrame)
	{
		//SE
		coll->CharacterOnceSE("UltLaser");
		coll->CharacterOnceSE("UltShot");
		//�U���쐬
		CreateAttack(actorManager);
	}
	//�U����������Z�b�g
	if (m_animCountFrame % kUltResetFrame == 0)
	{
		if (!m_attack.expired())m_attack.lock()->ResetHitId();
	}
	//��������������
	coll->GetRb()->SpeedDown(kMoveDeceRate);
	//�����̍X�V
	Vector2 dir = coll->GetPlayerStickVec();
	model->SetDir(dir);
	//���[�U�[�̈ʒu�X�V
	UpdateLaserPos();
}

void PlayerStateUltimate::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//�쐬�ƎQ��
	auto attack = std::dynamic_pointer_cast<ULT>(actorManager.lock()->CreateAttack(AttackType::ULT, m_owner).lock());
	//�U�����쐬
	auto data = m_attackData;
	//�傫��
	attack->SetRadius(data.radius);
	//�_���[�W�A�����t���[���A�m�b�N�o�b�N�̑傫���A�U���̏d���A�q�b�g�X�g�b�v�̒����A�J�����̗h��
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
	//�U���̎Q�Ƃ�ۑ�
	m_attack = attack;
	//�K�E�G�t�F�N�g
	auto eff = EffekseerManager::GetInstance().CreateEffect("UltLaserEff", m_owner.lock()->GetPos());
	//�G�t�F�N�g�̎Q�Ƃ�ۑ�
	m_laserEff = eff; 
	//�U���̈ʒu���X�V
	UpdateLaserPos();
}

void PlayerStateUltimate::UpdateLaserPos()
{
	auto owner = m_owner.lock();
	//�U���̈ʒu
	Vector3 startPos = owner->GetPos();
	Vector3 endPos = owner->GetPos() + (owner->GetModel()->GetDir() * kSwordHeight);
	if(!m_attack.expired())
	{
		//�U���̎Q��
		auto attack = m_attack.lock();
		//�U���̈ʒu���X�V
		attack->SetStartPos(startPos);
		attack->SetEndPos(endPos);
	}
	if(!m_laserEff.expired())
	{	
		auto eff = m_laserEff.lock();
		//�G�t�F�N�g�̈ʒu
		eff->SetPos(startPos);
		eff->LookAt(owner->GetModel()->GetDir());
	}
}

