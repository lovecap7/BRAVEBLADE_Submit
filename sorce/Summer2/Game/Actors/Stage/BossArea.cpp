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
	//アイテムの生成間隔
	constexpr int kItemDropInterval = 20 * 60;
	//アイテムの生成位置
	constexpr float kItemDropHeight = 2000.0f;
	//アイテム生成範囲
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
	//ボスを非アクティブ状態に
	boss.lock()->SetActive(false);
	//プレイヤーが入ったかチェック
	EventAreaBase::Update(camera, actorManager);
	//入ったなら
	if (m_isEvent)
	{
		//ボスをアクティブ状態に
		boss.lock()->SetActive(true);
		//ボスのUIの準備
		UIManager::GetInstance().CreateBossUI(boss.lock());
		InitEvent(actorManager, camera);
		//壁を出す
		SpawnWallEffect();
		return;
	}
}

void BossArea::EventUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//ボスが倒れたら
	if (actorManager.lock()->IsBossDisappear())
	{
		//壁削除
		if (!m_start.expired())m_start.lock()->Delete();
		if (!m_end.expired())m_end.lock()->Delete();

		//範囲内の敵がすべて消えたら
		//イベント終了
		m_isEvent = false;
		//このエリアも消す
		m_isDelete = true;
		return;
	}
	//プレイヤーがいない場合は何もしない
	if (actorManager.lock()->GetPlayer().expired())return;
	//アイテムを落とすフレームをカウント
	++m_itemDropFrame;
	if (m_itemDropFrame >= kItemDropInterval)
	{
		m_itemDropFrame = 0;
		//エリア内にアイテムを落とす
		if (m_start.expired() || m_end.expired())return;
		auto startPos = m_start.lock()->GetPos();
		auto endPos = m_end.lock()->GetPos();
		//エリア中央当たりから円形の範囲内に落とす
		Vector3 centerPos = (startPos + endPos) * 0.5f;
		//範囲内に収まるようにランダムな位置を決定
		Vector3 dropPos = centerPos;
		dropPos.x += MyMath::GetRandF(-kItemDropRange, kItemDropRange);
		dropPos.z += MyMath::GetRandF(-kItemDropRange, kItemDropRange);
		//高いところから落とす
		dropPos.y = kItemDropHeight;
		//アイテムを落とす
		actorManager.lock()->CreateRandItem(dropPos);
	}
}


void BossArea::InitEvent(const std::weak_ptr<ActorManager>& actorManager, const std::weak_ptr<GameCamera>& camera)
{
	//BGM変更
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
	//壁は閉ざす
	m_start.lock()->SetIsThrough(false);
	m_end.lock()->SetIsThrough(false);
	//イベント開始情報をカメラに設定
	camera.lock()->SetEventArea(std::dynamic_pointer_cast<BossArea>(shared_from_this()));
	//ボス以外の雑魚敵を削除
	actorManager.lock()->AllDeleteNormalEnemy();
	m_update = &BossArea::EventUpdate;
}