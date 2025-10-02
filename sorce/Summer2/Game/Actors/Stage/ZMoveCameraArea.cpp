#include "ZMoveCameraArea.h"
#include "../ActorManager.h"
#include "../../Camera/GameCamera/GameCamera.h"
ZMoveCameraArea::ZMoveCameraArea(std::weak_ptr<StageObjectCollision> start, std::weak_ptr<StageObjectCollision> end):
	EventAreaBase(start, end,AreaTag::ZMove)
{
}

ZMoveCameraArea::~ZMoveCameraArea()
{
}

void ZMoveCameraArea::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	EventAreaBase::Update(camera, actorManager);
	if (m_isEvent)
	{
		if (!camera.lock()->GetEventArea().expired())return;
		//イベント開始情報をカメラに設定
		camera.lock()->SetEventArea(std::dynamic_pointer_cast<ZMoveCameraArea>(shared_from_this()));
	}
}
