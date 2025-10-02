#include "GameCameraStateBossStart.h"
#include "GameCamera.h"
#include "GameCameraStateNormal.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/HitPoints.h"
#include "../../../General/game.h"
#include "../../Actors/Character/Player/Player.h"
#include "../../Actors/Character/Enemy/EnemyBase.h"
#include "../../Actors/Stage/BossArea.h"
#include "../../Actors/ActorManager.h"
#include "../../Actors/Stage/EventAreaBase.h"
#include "../../Actors/Character/Enemy/BossKing/BossKing.h"
#include <DxLib.h>

namespace
{
	//視野角
	constexpr float kPerspective = 35.0f * MyMath::DEG_2_RAD;
	//カメラ角度
	constexpr float kCameraAngleX = 0.0f * MyMath::DEG_2_RAD;
	//lerpの割合
	constexpr float kLerpRate = 0.07f;
	//ターゲットから少し離れるためのオフセット
	constexpr float kOffsetCameraPosY = 120.0f;
	constexpr float kOffsetCameraPosX = 100.0f;
	constexpr float kCameraPosZ = -100.0f;
	//見てる位置
	constexpr float kOffsetCameraViewPosY = 100.0f;
	//nearとfar
	constexpr float kNear = 300.0f;
	constexpr float kFar = 20000.0f;
}

GameCameraStateBossStart::GameCameraStateBossStart(std::weak_ptr<GameCamera> camera) :
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
	SetupCamera_Perspective(kPerspective);
	//奥行
	SetCameraNearFar(kNear, kFar);
}

void GameCameraStateBossStart::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void GameCameraStateBossStart::Update(const std::weak_ptr<ActorManager> actorManager)
{
	auto boss = actorManager.lock()->GetBoss();
	//カメラがあるかチェック
	if (m_camera.expired())return;
	auto camera = m_camera.lock();
	//プレイヤーが消滅した場合更新終了
	if (boss.expired())return;
	auto king = std::dynamic_pointer_cast<BossKing>(boss.lock());
	//プレイヤーのスタート状態が終わったらカメラを通常状態に
	if (!king->IsStartAnim())
	{
		ChangeState(std::make_shared<GameCameraStateNormal>(m_camera));
		return;
	}
	//ボス
	auto bossPos = king->GetRb()->GetPos();
	//位置の更新
	Vector3 oldPos = camera->GetPos();
	Vector3 nextPos = camera->GetPos();
	nextPos.z = kCameraPosZ;
	nextPos.y = bossPos.y + kOffsetCameraPosY;
	nextPos.x = bossPos.x + kOffsetCameraPosX;
	//見てる位置
	Vector3 viewPos = bossPos;
	viewPos.y += kOffsetCameraViewPosY;
	//位置更新
	SetCameraPositionAndTarget_UpVecY(nextPos.ToDxLibVector(), viewPos.ToDxLibVector());
	camera->SetPos(nextPos);
	camera->SetViewPos(viewPos);
}

