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
	//範囲内の敵をlistに入れる
	CheckAreaEnemies();
	//範囲内の敵をすべて非アクティブ化
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
		//壁を出す
		SpawnWallEffect();
		return;
	}
}

void AllKillArea::EventUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//範囲内の敵がすべて死亡した場合イベントを終了
	for (auto coll : m_areaEnemies)
	{
		//範囲内の敵がまだ生きているなら
		if (!coll.expired())return;
	}
	//範囲内の敵がすべて消えたら
	//イベント終了
	m_isEvent = false;
	//削除
	m_isDelete = true;
	//壁も削除
	m_start.lock()->Delete();
	m_end.lock()->Delete();
}

void AllKillArea::InitEvent(const std::weak_ptr<GameCamera> camera)
{
	//範囲内の敵をすべてアクティブ化
	for (auto& enemy : m_areaEnemies)
	{
		if (enemy.expired())continue;
		enemy.lock()->SetActive(true);
	}
	//壁は閉ざす
	std::dynamic_pointer_cast<StageObjectCollision>(m_start.lock())->SetIsThrough(false);
	std::dynamic_pointer_cast<StageObjectCollision>(m_end.lock())->SetIsThrough(false);
	//イベント開始情報をカメラに設定
	camera.lock()->SetEventArea(std::dynamic_pointer_cast<AllKillArea>(shared_from_this()));
	//更新処理の状態変更
	m_update = &AllKillArea::EventUpdate;
}

void AllKillArea::CheckAreaEnemies()
{
	auto startPos = m_start.lock()->GetPos();
	auto endPos = m_end.lock()->GetPos();
	//範囲内のCollidableの参照を取得
	auto collList = Physics::GetInstance().GetAreaXCollidable(startPos.x, endPos.x);
	for (auto& coll : collList)
	{
		//範囲内の敵の参照を取得
		if (coll.expired())continue;
		if (coll.lock()->GetGameTag() == GameTag::Enemy)
		{
			//敵をカウントしていく
			m_areaEnemies.emplace_back(std::dynamic_pointer_cast<EnemyBase>(coll.lock()));
		}
	}
}
