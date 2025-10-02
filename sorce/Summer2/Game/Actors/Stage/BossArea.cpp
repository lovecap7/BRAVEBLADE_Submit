#include "BossArea.h"
#include "StageObjectCollision.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../../UI/UIManager.h"
#include "../../../General/Collision/Physics.h"
#include "../../../General/Sound/SoundManager.h"
#include "../../Camera/GameCamera/GameCamera.h"

namespace
{
	//�A�C�e���̐����Ԋu
	constexpr int kItemDropInterval = 20 * 60;
	//�A�C�e���̐����ʒu
	constexpr float kItemDropHeight = 2000.0f;
	//�A�C�e�������͈�
	constexpr float kItemDropRange = 600.0f;
}

BossArea::BossArea(std::weak_ptr<StageObjectCollision> start, std::weak_ptr<StageObjectCollision> end):
	EventAreaBase(start,end,AreaTag::Boss),
	m_update(&BossArea::EntryCheckUpdate),
	m_itemDropFrame(0)
{
}

BossArea::~BossArea()
{
}

void BossArea::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	(this->*m_update)(camera, actorManager);
}

void BossArea::EntryCheckUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	if (actorManager.expired())return;
	auto boss = actorManager.lock()->GetBoss();
	if (boss.expired())return;
	//�{�X���A�N�e�B�u��Ԃ�
	boss.lock()->SetActive(false);
	//�v���C���[�����������`�F�b�N
	EventAreaBase::Update(camera, actorManager);
	//�������Ȃ�
	if (m_isEvent)
	{
		//�{�X���A�N�e�B�u��Ԃ�
		boss.lock()->SetActive(true);
		//�{�X��UI�̏���
		UIManager::GetInstance().CreateBossUI(boss.lock());
		InitEvent(actorManager, camera);
		//�ǂ��o��
		SpawnWallEffect();
		return;
	}
}

void BossArea::EventUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�{�X���|�ꂽ��
	if (actorManager.lock()->IsBossDisappear())
	{
		//�Ǎ폜
		if (!m_start.expired())m_start.lock()->Delete();
		if (!m_end.expired())m_end.lock()->Delete();

		//�͈͓��̓G�����ׂď�������
		//�C�x���g�I��
		m_isEvent = false;
		//���̃G���A������
		m_isDelete = true;
		return;
	}
	//�v���C���[�����Ȃ��ꍇ�͉������Ȃ�
	if (actorManager.lock()->GetPlayer().expired())return;
	//�A�C�e���𗎂Ƃ��t���[�����J�E���g
	++m_itemDropFrame;
	if (m_itemDropFrame >= kItemDropInterval)
	{
		m_itemDropFrame = 0;
		//�G���A���ɃA�C�e���𗎂Ƃ�
		if (m_start.expired() || m_end.expired())return;
		auto startPos = m_start.lock()->GetPos();
		auto endPos = m_end.lock()->GetPos();
		//�G���A���������肩��~�`�͈͓̔��ɗ��Ƃ�
		Vector3 centerPos = (startPos + endPos) * 0.5f;
		//�͈͓��Ɏ��܂�悤�Ƀ����_���Ȉʒu������
		Vector3 dropPos = centerPos;
		dropPos.x += MyMath::GetRandF(-kItemDropRange, kItemDropRange);
		dropPos.z += MyMath::GetRandF(-kItemDropRange, kItemDropRange);
		//�����Ƃ��납�痎�Ƃ�
		dropPos.y = kItemDropHeight;
		//�A�C�e���𗎂Ƃ�
		actorManager.lock()->CreateRandItem(dropPos);
	}
}


void BossArea::InitEvent(const std::weak_ptr<ActorManager>& actorManager, const std::weak_ptr<GameCamera>& camera)
{
	//BGM�ύX
	auto& soundManager = SoundManager::GetInstance();
	switch (actorManager.lock()->GetStageIndex())
	{
	case Stage::StageIndex::Stage1:
		soundManager.PlayBGM("Stage1_BossBGM");
		break;
	case Stage::StageIndex::Stage2:
		soundManager.PlayBGM("Stage2_BossBGM");
		break;
	case Stage::StageIndex::Stage3:
		soundManager.PlayBGM("Stage3_BossBGM");
		break;
	}
	//�ǂ͕���
	m_start.lock()->SetIsThrough(false);
	m_end.lock()->SetIsThrough(false);
	//�C�x���g�J�n�����J�����ɐݒ�
	camera.lock()->SetEventArea(std::dynamic_pointer_cast<BossArea>(shared_from_this()));
	//�{�X�ȊO�̎G���G���폜
	actorManager.lock()->AllDeleteNormalEnemy();
	m_update = &BossArea::EventUpdate;
}