#include "Heart.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/HitPoints.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Sound/SoundManager.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../../GameRule/Score.h"
namespace
{
	//�񕜗�
	constexpr float kHealValue = 400.0f;
	//�W�����v��
	constexpr float kJumpPower = 10.0f;
	//�����蔻��̔��a
	constexpr float kCollRadius = 80.0f;
}

Heart::Heart(int modelHandle, Vector3 pos):
	ItemBase(Shape::Sphere)
{
	//���W
	auto firstPos = pos;
	firstPos.y += kCollRadius;
	m_rb->m_pos = firstPos;
	std::dynamic_pointer_cast<SphereCollider>(m_collisionData)->SetRadius(kCollRadius);
	//���f��
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//�͂�^����
	m_rb->SetVecY(kJumpPower);
}

Heart::~Heart()
{
}

void Heart::OnCollide(const std::shared_ptr<Collidable> other)
{
	//���Ńt���O�������Ă�Ȃ烊�^�[��
	if (m_isDelete)return;
	//�ړ��ʂ�������
	m_rb->SetVec(Vector3::Zero());
	//�v���C���[�ɓ����������̏���
	if (other->GetGameTag() == GameTag::Player)
	{
		//SE
		SoundManager::GetInstance().PlayOnceSE("GetItem");
		//��
		auto player = std::dynamic_pointer_cast<Player>(other);
		player->GetHitPoints().lock()->Heal(kHealValue);
		//�񕜃G�t�F�N�g
		EffekseerManager::GetInstance().CreateTrackActorEffect("GetHealEff", player);
		//�폜
		m_isDelete = true;
	}
}

void Heart::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	//�X�R�A���Z
	score.lock()->AddKillOrItemScore(ScoreDataName::kHeart);
}
