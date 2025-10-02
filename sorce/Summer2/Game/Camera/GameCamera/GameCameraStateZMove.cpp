#include "GameCameraStateZMove.h"
#include "GameCameraStateNormal.h"
#include "GameCameraStateAreaLock.h"
#include "GameCameraStateBossArea.h"
#include "GameCameraStateBossDeath.h"
#include "GameCameraStateClear.h"
#include "../../Actors/Stage/EventAreaBase.h"
#include "GameCamera.h"
#include "../../Actors/Character/Player/Player.h"
#include "../../Actors/ActorManager.h"
#include <DxLib.h>
namespace
{
	//視野角
	constexpr float kPerspective = 35.0f * MyMath::DEG_2_RAD;
	//カメラ角度
	constexpr float kCameraAngleX = 30.0f * MyMath::DEG_2_RAD;
	//lerpの割合
	constexpr float kLerpRateX = 0.1f;
	constexpr float kLerpRateY = 0.1f;
	constexpr float kLerpRateZ = 0.1f;
	//ターゲットから少し離れるためのオフセット
	constexpr float kOffsetCameraPosY = 900.0f;
	constexpr float kOffsetCameraPosZ = -1400.0f;
	//nearとfar
	constexpr float kNear = 300.0f;
	constexpr float kFar = 35000.0f;
}

GameCameraStateZMove::GameCameraStateZMove(std::weak_ptr<GameCamera> camera):
	GameCameraStateBase(camera)
{
	//カメラがあるかチェック
	if (m_camera.expired())return;
	auto owner = m_camera.lock();
	//カメラの角度
	owner->SetDir(Matrix4x4::RotateXMat4x4(kCameraAngleX) *
		Vector3::Forward());
	//見てる位置
	owner->SetViewPos(owner->GetPos() + owner->GetDir());
	//カメラの座標と注視点
	DxLib::SetCameraPositionAndTarget_UpVecY(owner->GetPos().ToDxLibVector(), owner->GetViewPos().ToDxLibVector());
	//視野角
	DxLib::SetupCamera_Perspective(kPerspective);
	//奥行
	SetCameraNearFar(kNear, kFar);
}

void GameCameraStateZMove::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void GameCameraStateZMove::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//ボスが死亡した場合
	if (actorManager.lock()->IsBossDead())
	{
		ChangeState(std::make_shared<GameCameraStateBossDeath>(m_camera, actorManager));
		return;
	}
	//カメラがあるかチェック
	if (m_camera.expired())return;
	auto camera = m_camera.lock();

	if (!camera->GetEventArea().expired())
	{
		//イベント終了
		if (!camera->GetEventArea().lock()->IsEvent())
		{
			//通常のカメラへ
			ChangeState(std::make_shared<GameCameraStateNormal>(m_camera));
			return;
		}
		//イベントエリアのタグによって状態を変える
		switch (camera->GetEventArea().lock()->GetAreaTag())
		{
		case AreaTag::AllKill:
			//全滅エリア
			ChangeState(std::make_shared<GameCameraStateAreaLock>(m_camera));
			return;
		case AreaTag::Boss:
			//ボスエリア
			ChangeState(std::make_shared<GameCameraStateBossArea>(m_camera));
			return;
		}

		auto player = actorManager.lock()->GetPlayer();
		//プレイヤーか消滅した場合更新終了
		if (player.expired())return;
		//エリアの始点と終点
		auto area = camera->GetEventArea().lock();
		auto startPos = area->GetEventStartPos();
		auto endPos = area->GetEventEndPos();
		//プレイヤーの座標
		auto playerPos = player.lock()->GetPos();
		//位置の更新
		Vector3 oldPos = camera->GetPos();
		Vector3 nextPos = camera->GetPos();
		nextPos.y = playerPos.y + kOffsetCameraPosY;//プレイヤーのY座標より高い位置
		//エリアの外にカメラが近づいたら止まる
		nextPos.x = playerPos.x;
		nextPos.z = playerPos.z + kOffsetCameraPosZ;
		//次の座標
		nextPos.x = MathSub::Lerp(oldPos.x, nextPos.x, kLerpRateX);
		nextPos.y = MathSub::Lerp(oldPos.y, nextPos.y, kLerpRateY);
		nextPos.z = MathSub::Lerp(oldPos.z, nextPos.z, kLerpRateZ);
		//見ている向き
		Vector3 dir = camera->GetDir();
		//見てる位置
		Vector3 viewPos = nextPos + dir;
		//位置更新
		DxLib::SetCameraPositionAndTarget_UpVecY(nextPos.ToDxLibVector(), viewPos.ToDxLibVector());
		camera->SetPos(nextPos);
		camera->SetDir(dir);
		camera->SetViewPos(viewPos);
	}
	//イベントが消滅したら
	else
	{
		//通常のカメラへ
		ChangeState(std::make_shared<GameCameraStateNormal>(m_camera));
		return;
	}
}
