#include "AttackUp.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/AttackPoints.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Effect/TrackActorEffect.h"
#include "../../../General/Sound/SoundManager.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../../GameRule/Score.h"

namespace
{
	//�_���[�W�{��
	constexpr float kDamageRate = 1.5f;
	//�����t���[��
	constexpr int kAttackUpKeepFrame = 10 * 60;
	//�W�����v��
	constexpr float kJumpPower = 10.0f;
	//�����蔻��̔��a
	constexpr float kCollRadius = 80.0f;
}

AttackUp::AttackUp(int modelHandle, Vector3 pos) :
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

AttackUp::~AttackUp()
{
}

void AttackUp::OnCollide(const std::shared_ptr<Collidable> other)
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
		//�U���A�b�v�o�t
		auto player = std::dynamic_pointer_cast<Player>(other);
		player->GetAttackPoints().lock()->AttackBuff(kDamageRate, Battle::AttackWeight::Heavy, kAttackUpKeepFrame);
		//�p���[�A�b�v�G�t�F�N�g
		EffekseerManager::GetInstance().CreateTrackActorEffect("GetAttackUpEff", player);
		auto eff = EffekseerManager::GetInstance().CreateTrackActorEffect("AttackUpEff", player);
		eff.lock()->SpecificFrame(kAttackUpKeepFrame);
		//�폜
		m_isDelete = true;
	}
}

void AttackUp::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	//�X�R�A���Z
	score.lock()->AddKillOrItemScore(ScoreDataName::kAttackUp);
}
