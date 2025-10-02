#pragma once
#include "MyEffect.h"
#include <memory>
class Actor;
//持ち主の座標に合わせて移動(追跡)するクラス
class TrackActorEffect :
    public MyEffect
{
public:
	TrackActorEffect(int playHandle,std::weak_ptr<Actor> target);
	~TrackActorEffect();
	void Update() override;			//更新
private:
	//追跡ターゲット
	std::weak_ptr<Actor> m_target;
};

