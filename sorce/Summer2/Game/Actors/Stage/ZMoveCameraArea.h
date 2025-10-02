#pragma once
#include "EventAreaBase.h"
#include <list>
//�v���C���[���͈͓��ɓ������������m���C�x���g���I���܂Ń`�F�b�N����
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

