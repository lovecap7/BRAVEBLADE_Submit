#include "GameCameraStateClear.h"
#include "GameCamera.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/game.h"
#include "../../Actors/Character/Player/Player.h"
#include "../../Actors/Character/Enemy/EnemyBase.h"
#include "../../Actors/ActorManager.h"
#include <DxLib.h>

namespace
{
	//カメラ角度
	constexpr float kNormalCameraAngleX = 30.0f * MyMath::DEG_2_RAD;
	constexpr float kBossCameraAngleX = 40.0f * MyMath::DEG_2_RAD;
	//画面中央からある一定距離プレイヤーが離れた場合追従する範囲
	constexpr float kChaseWidth = 20.0f;
	//lerpの割合
	constexpr float kClearEndLerpRate = 0.05f;
	//ターゲットから少し離れるためのオフセット
	//クリアして回転してるとき
	constexpr float kOffsetClearStartCameraPosY = 50.0f;
	constexpr float kOffsetClearStartCameraPosZ = -500.0f;
	//回転終了後のアップ
	constexpr float kOffsetClearEndCameraPosY = 130.0f;
	constexpr float kOffsetClearEndCameraPosZ = -200.0f;
	//クリア時のターゲットへの注視点オフセット
	constexpr float kOffsetClearStartViewPosY = 100.0f;
	constexpr float kOffsetClearEndViewPosY = 100.0f;

	//クリア時に回転するフレーム
	constexpr int kClearRotaFrame = 400;
	//nearとfar
	constexpr float kNear = 10.0f;
	constexpr float kFar = 10000.0f;
}
GameCameraStateClear::GameCameraStateClear(std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager):
	GameCameraStateBase(camera),
	m_rotaFrame(kClearRotaFrame)
{
	//カメラがあるかチェック
	if (m_camera.expired())return;
	auto owner = m_camera.lock();
	auto player = actorManager.lock()->GetPlayer();
	auto playerPos = player.lock()->GetNextPos();
	//位置の更新
	Vector3 nextPos = playerPos;
	nextPos.y += kOffsetClearStartCameraPosY;
	nextPos.z += kOffsetClearStartCameraPosZ;
	owner->SetPos(nextPos);
	//見てる位置
	auto viewPos = playerPos;
	viewPos.y += kOffsetClearStartViewPosY;
	owner->SetViewPos(viewPos);
	//奥行
	SetCameraNearFar(kNear, kFar);
}

void GameCameraStateClear::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void GameCameraStateClear::Update(const std::weak_ptr<ActorManager> actorManager)
{
	auto player = actorManager.lock()->GetPlayer();
	auto camera = m_camera.lock();
	auto playerPos = player.lock()->GetPos();
	auto viewPos = camera->GetViewPos();
	//次の座標
	if (m_rotaFrame > 0)
	{
		Quaternion kClearRotaQ = Quaternion::AngleAxis((360.0f / kClearRotaFrame) * MyMath::DEG_2_RAD, Vector3::Up());
		//位置の更新
		Vector3 nextPos = playerPos;
		auto vec = camera->GetPos() - playerPos;
		vec = kClearRotaQ * vec;
		nextPos += vec;
		nextPos.y += 0.5f;
		camera->SetPos(nextPos);
		--m_rotaFrame;
	}
	else
	{
		//位置の更新
		Vector3 nextPos = playerPos;
		nextPos.z += kOffsetClearEndCameraPosZ;
		nextPos.y += kOffsetClearEndCameraPosY;
		//次の座標
		nextPos = Vector3::Lerp(camera->GetPos(), nextPos, kClearEndLerpRate);
		camera->SetPos(nextPos);
		//注視点
		viewPos = playerPos;
		viewPos.y += kOffsetClearEndViewPosY;
		camera->SetViewPos(viewPos);
	}
	//設定
	SetCameraPositionAndTarget_UpVecY(camera->GetPos().ToDxLibVector(), viewPos.ToDxLibVector());
}
