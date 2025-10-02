#pragma once
#include "../Stage/StageObjectCollision.h"

enum class AreaTag
{
    AllKill, //全滅エリア
    ZMove,   //Z軸移動エリア
	Boss,    //ボスエリア
};

class GameCamera;
class EventAreaBase abstract:
    public Actor
{
public:
    EventAreaBase(std::weak_ptr<StageObjectCollision> start, std::weak_ptr<StageObjectCollision> end, AreaTag area);
    virtual ~EventAreaBase();
    virtual void Init()override;
    virtual void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    virtual void OnCollide(const std::shared_ptr<Collidable> other)override {};
    virtual void Draw()const override {};
    virtual void Complete() override {};
    virtual void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override {};
    virtual void End()override {};
    //イベント中
    bool IsEvent()const { return m_isEvent; };
    //スタートとエンドのX座標
    Vector3 GetEventStartPos()const { return m_start.lock()->GetPos(); };
    Vector3 GetEventEndPos()const { return m_end.lock()->GetPos(); };
    //エリア
	AreaTag GetAreaTag()const { return m_areaTag; };
    //エフェクトで壁を出す
	void SpawnWallEffect();
protected:
    std::weak_ptr<StageObjectCollision> m_start;
    std::weak_ptr<StageObjectCollision> m_end;
    //イベント中フラグ
    bool m_isEvent;
	//イベントのタグ
    AreaTag m_areaTag;
};