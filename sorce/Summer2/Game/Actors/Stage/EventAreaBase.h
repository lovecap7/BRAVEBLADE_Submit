#pragma once
#include "../Stage/StageObjectCollision.h"

enum class AreaTag
{
    AllKill, //�S�ŃG���A
    ZMove,   //Z���ړ��G���A
	Boss,    //�{�X�G���A
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
    //�C�x���g��
    bool IsEvent()const { return m_isEvent; };
    //�X�^�[�g�ƃG���h��X���W
    Vector3 GetEventStartPos()const { return m_start.lock()->GetPos(); };
    Vector3 GetEventEndPos()const { return m_end.lock()->GetPos(); };
    //�G���A
	AreaTag GetAreaTag()const { return m_areaTag; };
    //�G�t�F�N�g�ŕǂ��o��
	void SpawnWallEffect();
protected:
    std::weak_ptr<StageObjectCollision> m_start;
    std::weak_ptr<StageObjectCollision> m_end;
    //�C�x���g���t���O
    bool m_isEvent;
	//�C�x���g�̃^�O
    AreaTag m_areaTag;
};