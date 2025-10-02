#pragma once
#include "EventAreaBase.h"
#include <list>
//プレイヤーが範囲内に入ったかを検知しイベントが終了までチェックする
class GameCamera;
class ActorManager;
class EnemyBase;
class StageObjectCollision;
class AllKillArea :
	public EventAreaBase
{
public:
    AllKillArea(std::weak_ptr<StageObjectCollision> start, std::weak_ptr<StageObjectCollision> end);
    virtual ~AllKillArea();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //状態遷移
    using UpdateFunc_t = void(AllKillArea::*)(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager);
    UpdateFunc_t m_update;
    //範囲チェック状態
    void EntryCheckUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager);
    //イベント状態
    void EventUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager);
    //初期化
    void InitEvent(const std::weak_ptr<GameCamera> camera);
    void CheckAreaEnemies();
    //範囲内の敵のリスト
    std::list<std::weak_ptr<EnemyBase>> m_areaEnemies;
};

