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
    //��ԑJ��
    using UpdateFunc_t = void(BossArea::*)(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager);
    UpdateFunc_t m_update;
    //�͈̓`�F�b�N���
    void EntryCheckUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager);
    //�C�x���g���
    void EventUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager);
    //������
    void InitEvent(const std::weak_ptr<ActorManager>& actorManager, const std::weak_ptr<GameCamera>& camera);

private:
    //��莞�Ԃ��ƂɃA�C�e���𗎂Ƃ�
	int m_itemDropFrame;
};

