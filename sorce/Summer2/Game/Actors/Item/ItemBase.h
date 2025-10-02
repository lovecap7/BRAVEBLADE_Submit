#pragma once
#include "../Actor.h"

class ActorManager;
class ItemBase abstract:
    public Actor
{
public:
    ItemBase(Shape shape);
    ~ItemBase();
	//初期化処理
	void Init()override;
	//更新処理
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//描画
	void Draw()const override;
	//更新処理の確定
	void Complete() override;
	//終了処理
	void End()override;
protected:
    //最初の数フレームは当たり判定をしたくないので
    int m_noHitFrame;
};

