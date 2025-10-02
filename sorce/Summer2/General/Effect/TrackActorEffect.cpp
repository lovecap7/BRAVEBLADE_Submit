#include "TrackActorEffect.h"
#include "../../Game/Actors/Actor.h"

TrackActorEffect::TrackActorEffect(int playHandle, std::weak_ptr<Actor> target):
	MyEffect(playHandle, target.lock()->GetPos()),
	m_target(target)
{
}

TrackActorEffect::~TrackActorEffect()
{
}

void TrackActorEffect::Update()
{
	//ターゲットがいないなら削除
	if (m_target.expired())
	{
		Delete();
		return;
	}
	//移動
	SetPos(m_target.lock()->GetPos());
	//更新
	MyEffect::Update();
}
