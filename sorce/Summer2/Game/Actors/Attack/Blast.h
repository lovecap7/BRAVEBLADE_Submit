#pragma once
#include "SphereAttackBase.h"
//範囲攻撃
class Actor;
class Blast :
    public SphereAttackBase
{
public:
    Blast(std::weak_ptr<Actor> owner);
    ~Blast();
    //初期化
    void Init() override;
    //更新処理
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    void OnCollide(const std::shared_ptr<Collidable> other)override;
    //描画
    void Draw()const override;
    //ノックバック
    Vector3 GetKnockBackVec(Position3 other) override;
};
