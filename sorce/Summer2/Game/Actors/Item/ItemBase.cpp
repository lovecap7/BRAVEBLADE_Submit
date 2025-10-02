#include "ItemBase.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/HitPoints.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Effect/TrackActorEffect.h"
#include "../../../General/Sound/SoundManager.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../../GameRule/Score.h"

namespace
{
	//�_���[�W�J�b�g
	constexpr float kDamageCutRate = 0.5f;
	//�����t���[��
	constexpr int kDamageCutKeepFrame = 60 * 20;//15�b
	//�W�����v��
	constexpr float kJumpPower = 10.0f;
	//�����蔻��̔��a
	constexpr float kCollRadius = 80.0f;
	//��]��
	constexpr float kRotaAngle = 1.0f;
	//�ŏ��̓�����Ȃ��t���[��
	constexpr int kNoHitFrame = 30;
}

ItemBase::ItemBase(Shape shape):
	Actor(shape),
	m_noHitFrame(kNoHitFrame)
{
}

ItemBase::~ItemBase()
{
}

void ItemBase::Init()
{
	//�R���C�_�u���̏�����
	AllSetting(CollisionState::Normal, Priority::Low, GameTag::Item, true, false, true);
	//Physics�ɓo�^
	Collidable::Init();
}

void ItemBase::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�ړ��ʂ�������
	m_rb->SetMoveVec(Vector3::Zero());
	//���
	m_model->SetRot(VGet(0.0f, kRotaAngle, 0.0f));
	if (m_noHitFrame > 0)
	{
		--m_noHitFrame;
	}
	else
	{
		//�����蔻�������
		m_isThrough = false;
	}
	m_model->Update();
}

void ItemBase::Draw() const
{
#if _DEBUG
	//�Փ˔���
	DrawSphere3D(
		GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<SphereCollider>(m_collisionData)->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		false
	);
#endif
	m_model->Draw();
}

void ItemBase::Complete()
{
	//���̍��W��
	m_rb->m_pos = m_rb->GetNextPos();
	//���f���̍��W�X�V
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
}

void ItemBase::End()
{
	Collidable::End();
	m_model->End();
}
