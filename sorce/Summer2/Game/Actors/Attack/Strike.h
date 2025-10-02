#pragma once
#include "CapsuleAttackBase.h"
class Strike :
    public CapsuleAttackBase
{
public:
    Strike(std::weak_ptr<Actor> owner);
    ~Strike();
    //�X�V����
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    //�`��
    void Draw()const override;
    //����������
    void OnCollide(const std::shared_ptr<Collidable> other) override;
};

