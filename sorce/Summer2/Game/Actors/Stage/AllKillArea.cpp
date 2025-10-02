#include "AllKillArea.h"
#include "StageObjectCollision.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../../../General/Collision/Physics.h"
#include "../../Camera/GameCamera/GameCamera.h"

AllKillArea::AllKillArea(std::weak_ptr<StageObjectCollision> start, std::weak_ptr<StageObjectCollision> end):
	EventAreaBase(start,end,AreaTag::AllKill),
	m_update(&AllKillArea::EntryCheckUpdate)
{
}

AllKillArea::~AllKillArea()
{
}

void AllKillArea::Init()
{
	//�͈͓��̓G��list�ɓ����
	CheckAreaEnemies();
	//�͈͓��̓G�����ׂĔ�A�N�e�B�u��
	for (auto& enemy : m_areaEnemies)
	{
		if (enemy.expired())continue;
		enemy.lock()->SetActive(false);
	}
}


void AllKillArea::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	(this->*m_update)(camera,actorManager);
}
void AllKillArea::EntryCheckUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	EventAreaBase::Update(camera, actorManager);
	if (m_isEvent)
	{
		InitEvent(camera);
		//�ǂ��o��
		SpawnWallEffect();
		return;
	}
}

void AllKillArea::EventUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�͈͓��̓G�����ׂĎ��S�����ꍇ�C�x���g���I��
	for (auto coll : m_areaEnemies)
	{
		//�͈͓��̓G���܂������Ă���Ȃ�
		if (!coll.expired())return;
	}
	//�͈͓��̓G�����ׂď�������
	//�C�x���g�I��
	m_isEvent = false;
	//�폜
	m_isDelete = true;
	//�ǂ��폜
	m_start.lock()->Delete();
	m_end.lock()->Delete();
}

void AllKillArea::InitEvent(const std::weak_ptr<GameCamera> camera)
{
	//�͈͓��̓G�����ׂăA�N�e�B�u��
	for (auto& enemy : m_areaEnemies)
	{
		if (enemy.expired())continue;
		enemy.lock()->SetActive(true);
	}
	//�ǂ͕���
	std::dynamic_pointer_cast<StageObjectCollision>(m_start.lock())->SetIsThrough(false);
	std::dynamic_pointer_cast<StageObjectCollision>(m_end.lock())->SetIsThrough(false);
	//�C�x���g�J�n�����J�����ɐݒ�
	camera.lock()->SetEventArea(std::dynamic_pointer_cast<AllKillArea>(shared_from_this()));
	//�X�V�����̏�ԕύX
	m_update = &AllKillArea::EventUpdate;
}

void AllKillArea::CheckAreaEnemies()
{
	auto startPos = m_start.lock()->GetPos();
	auto endPos = m_end.lock()->GetPos();
	//�͈͓���Collidable�̎Q�Ƃ��擾
	auto collList = Physics::GetInstance().GetAreaXCollidable(startPos.x, endPos.x);
	for (auto& coll : collList)
	{
		//�͈͓��̓G�̎Q�Ƃ��擾
		if (coll.expired())continue;
		if (coll.lock()->GetGameTag() == GameTag::Enemy)
		{
			//�G���J�E���g���Ă���
			m_areaEnemies.emplace_back(std::dynamic_pointer_cast<EnemyBase>(coll.lock()));
		}
	}
}
