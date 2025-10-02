#include "UltGageUp.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Sound/SoundManager.h"
#include "../Character/Player/UltGage.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../../GameRule/Score.h"

namespace
{
	//�Q�[�W�㏸��
	constexpr float kGageValue = 20.0f;
	//�W�����v��
	constexpr float kJumpPower = 10.0f;
	//�����蔻��̔��a
	constexpr float kCollRadius = 60.0f;
}

UltGageUp::UltGageUp(int modelHandle, Vector3 pos) :
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

UltGageUp::~UltGageUp()
{
}

void UltGageUp::OnCollide(const std::shared_ptr<Collidable> other)
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
		//�Q�[�W�A�b�v
		auto player = std::dynamic_pointer_cast<Player>(other);
		player->GetUltGage().lock()->AddUltGage(kGageValue);
		//�Q�[�W�A�b�v�G�t�F�N�g
		EffekseerManager::GetInstance().CreateTrackActorEffect("GetUltGageUpEff", player);
		//�폜
		m_isDelete = true;
	}
}

void UltGageUp::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	//�X�R�A���Z
	score.lock()->AddKillOrItemScore(ScoreDataName::kUltGageUp);
}
