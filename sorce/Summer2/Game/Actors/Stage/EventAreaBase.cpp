#include "EventAreaBase.h"
#include "StageObjectCollision.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../../../General/Collision/Physics.h"
#include "../../Camera/GameCamera/GameCamera.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Effect/TrackActorEffect.h"
#include "../../../General/Model.h"


EventAreaBase::EventAreaBase(std::weak_ptr<StageObjectCollision> start, std::weak_ptr<StageObjectCollision> end, AreaTag area):
	Actor(Shape::None),
	m_start(start),
	m_end(end),
	m_isEvent(false),
	m_areaTag(area)
{
}

EventAreaBase::~EventAreaBase()
{
}

void EventAreaBase::Init()
{
}

void EventAreaBase::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	if (actorManager.lock()->GetPlayer().expired())return;
	auto player = actorManager.lock()->GetPlayer().lock();
	//座標から範囲に入ったかをチェック
	auto playerPos = player->GetPos();
	if (m_start.expired() || m_end.expired())return;
	auto startPos = m_start.lock()->GetPos();
	auto endPos = m_end.lock()->GetPos();

	//範囲内にいるか
	Vector2 sToEVec = endPos.XZ() - startPos.XZ();
	Vector2 sToPVec = playerPos.XZ() - startPos.XZ();
	if (sToEVec.Magnitude() > 0.0f && sToPVec.Magnitude())
	{
		sToEVec = sToEVec.Normalize();
		sToPVec = sToPVec.Normalize();
	}
	//範囲内にいるかチェック
	m_isEvent = Vector2::IsPointInRect(startPos.XZ(), endPos.XZ(), playerPos.XZ());
	//壁はすり抜ける
	std::dynamic_pointer_cast<StageObjectCollision>(m_start.lock())->SetIsThrough(true);
	std::dynamic_pointer_cast<StageObjectCollision>(m_end.lock())->SetIsThrough(true);
}

void EventAreaBase::SpawnWallEffect()
{
	if (m_start.expired() || m_end.expired())return;
	auto& effectManager = EffekseerManager::GetInstance();
	auto startWallEff	= effectManager.CreateTrackActorEffect("AreaWallEff", m_start);
	auto endWallEff		= effectManager.CreateTrackActorEffect("AreaWallEff", m_end);
	//回転も反映
	if (!startWallEff.expired())
	{
		if (!m_start.lock()->GetModel().expired())
		{
			
			startWallEff.lock()->LookAt(m_start.lock()->GetModel().lock()->GetDir());
		}
	}
	if (!endWallEff.expired())
	{
		if (!m_end.lock()->GetModel().expired())
		{
			endWallEff.lock()->LookAt(m_end.lock()->GetModel().lock()->GetDir());
		}
	}
}
