#include "DefenseUp.h"
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
	constexpr int kDamageCutKeepFrame = 10 * 60;
	//�W�����v��
	constexpr float kJumpPower = 10.0f;
	//�����蔻��̔��a
	constexpr float kCollRadius = 80.0f;
}

DefenseUp::DefenseUp(int modelHandle, Vector3 pos) :
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

DefenseUp::~DefenseUp()
{
}


void DefenseUp::OnCollide(const std::shared_ptr<Collidable> other)
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
		//�_���[�W�J�b�g�ƃA�[�}�[����
		auto player = std::dynamic_pointer_cast<Player>(other);
		player->GetHitPoints().lock()->DefenseBuff(Battle::Armor::Heavy, kDamageCutRate, kDamageCutKeepFrame);
		//�f�B�t�F���X�A�b�v�G�t�F�N�g
		EffekseerManager::GetInstance().CreateTrackActorEffect("GetDefenseUpEff", player);
		auto eff = EffekseerManager::GetInstance().CreateTrackActorEffect("DefenseUpEff", player);
		eff.lock()->SpecificFrame(kDamageCutKeepFrame);
		//�폜
		m_isDelete = true;
	}
}

void DefenseUp::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	//�X�R�A���Z
	score.lock()->AddKillOrItemScore(ScoreDataName::kDefenseUp);
}
