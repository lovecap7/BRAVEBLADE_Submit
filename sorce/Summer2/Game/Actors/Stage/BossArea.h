#pragma once
#include "EventAreaBase.h"
class Actor;
class ActorManager;
class Collidable;
class StageObjectCollision;
class GameCamera;
class BossArea :
    public EventAreaBase
{
public:
    BossArea(std::weak_ptr<StageObjectCollision> start, std::weak_ptr<StageObjectCollision> end);
    ~BossArea();
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //状態遷移
    using UpdateFunc_t = void(BossArea::*)(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager);
    UpdateFunc_t m_update;
    //範囲チェック状態
    void EntryCheckUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager);
    //イベント状態
    void EventUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager);
    //初期化
    void InitEvent(const std::weak_ptr<ActorManager>& actorManager, const std::weak_ptr<GameCamera>& camera);

private:
    //一定時間ごとにアイテムを落とす
	int m_itemDropFrame;
};

