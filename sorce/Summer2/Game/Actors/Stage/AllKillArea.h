#pragma once
#include "EventAreaBase.h"
#include <list>
//�v���C���[���͈͓��ɓ������������m���C�x���g���I���܂Ń`�F�b�N����
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
    //��ԑJ��
    using UpdateFunc_t = void(AllKillArea::*)(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager);
    UpdateFunc_t m_update;
    //�͈̓`�F�b�N���
    void EntryCheckUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager);
    //�C�x���g���
    void EventUpdate(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager);
    //������
    void InitEvent(const std::weak_ptr<GameCamera> camera);
    void CheckAreaEnemies();
    //�͈͓��̓G�̃��X�g
    std::list<std::weak_ptr<EnemyBase>> m_areaEnemies;
};

