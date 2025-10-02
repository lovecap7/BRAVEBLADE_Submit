#pragma once
#include "EventAreaBase.h"
#include <list>
//プレイヤーが範囲内に入ったかを検知しイベントが終了までチェックする
class GameCamera;
class ActorManager;
class EnemyBase;
class StageObjectCollision;
class ZMoveCameraArea :
    public EventAreaBase
{
public:
    ZMoveCameraArea(std::weak_ptr<StageObjectCollision> start, std::weak_ptr<StageObjectCollision> end);
    virtual ~ZMoveCameraArea();
    virtual void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
};

