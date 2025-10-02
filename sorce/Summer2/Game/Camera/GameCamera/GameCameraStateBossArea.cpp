#include "GameCameraStateBossArea.h"
#include "GameCameraStateAreaLock.h"
#include "GameCameraStateClear.h"
#include "GameCameraStateNormal.h"
#include "GameCameraStateBossDeath.h"
#include "GameCamera.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/game.h"
#include "../../../General/HitPoints.h"
#include "../../Actors/Character/Player/Player.h"
#include "../../Actors/Character/Enemy/EnemyBase.h"
#include "../../Actors/ActorManager.h"
#include "../../Actors/Stage/EventAreaBase.h"
#include <DxLib.h>

namespace
{
	//視野角
	constexpr float kPerspective = 40.0f * MyMath::DEG_2_RAD;
	//カメラ角度
	constexpr float kCameraAngleX = 35.0f * MyMath::DEG_2_RAD;
	//lerpの割合
	constexpr float kLerpRateX = 0.1f;
	constexpr float kLerpRateY = 0.1f;
	constexpr float kLerpRateZ = 0.05f;
	//ターゲットから少し離れるためのオフセット
	constexpr float kOffsetCameraPosY = 800.0f;
	constexpr float kOffsetCameraPosZ = -1000.0f;
	//壁からの距離
	constexpr float kDistanceFromWall = 300.0f;
	//距離の反映率
	constexpr float kDistanceRate = 3.0f;
	//nearとfar
	constexpr float kNear = 300.0f;
	constexpr float kFar = 20000.0f;
}
GameCameraStateBossArea::GameCameraStateBossArea(std::weak_ptr<GameCamera> camera):
	GameCameraStateBase(camera),
	m_angle(kCameraAngleX)
{
	//カメラがあるかチェック
	if (m_camera.expired())return;
	auto owner = m_camera.lock();
	//見てる位置
	owner->SetViewPos(owner->GetPos() + owner->GetDir());
	//カメラの座標と注視点
	DxLib::SetCameraPositionAndTarget_UpVecY(owner->GetPos().ToDxLibVector(), owner->GetViewPos().ToDxLibVector());
	//視野角
	DxLib::SetupCamera_Perspective(kPerspective);
	//奥行
	SetCameraNearFar(kNear, kFar);
}

void GameCameraStateBossArea::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void GameCameraStateBossArea::Update(const std::weak_ptr<ActorManager> actorManager)
{
	
	//ボスが死亡した場合
	if (actorManager.lock()->IsBossDead())
	{
		ChangeState(std::make_shared<GameCameraStateBossDeath>(m_camera, actorManager));
		return;
	}
	auto camera = m_camera.lock();
	//イベントが消滅したら
	if (camera->GetEventArea().expired())
	{
		//通常のカメラへ
		ChangeState(std::make_shared<GameCameraStateNormal>(m_camera));
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
	auto boss = actorManager.lock()->GetBoss();
	//ボスがいないなら
	if (boss.expired())return;
	//間の位置
	Vector3 center = (boss.lock()->GetPos() + playerPos) / 2.0f;
	//プレイヤーとボスの距離
	float distance = (boss.lock()->GetPos() - playerPos).Magnitude();
	//位置の更新
	Vector3 oldPos = camera->GetPos();
	Vector3 nextPos = camera->GetPos();
	//間の距離応じてカメラを高く
	nextPos.y = center.y + kOffsetCameraPosY + (distance / kDistanceRate);//プレイヤーのY座標より高い位置
	//エリアの外にカメラが近づいたら止まる
	nextPos.x = center.x;
	if (nextPos.x <= startPos.x + kDistanceFromWall)
	{
		nextPos.x = startPos.x + kDistanceFromWall;
	}
	else if (nextPos.x >= endPos.x - kDistanceFromWall)
	{
		nextPos.x = endPos.x - kDistanceFromWall;
	}
	//間の距離応じてカメラを引く
	nextPos.z = center.z + kOffsetCameraPosZ - (distance / kDistanceRate);
	//次の座標
	nextPos.x = MathSub::Lerp(oldPos.x, nextPos.x, kLerpRateX);
	nextPos.y = MathSub::Lerp(oldPos.y, nextPos.y, kLerpRateY);
	nextPos.z = MathSub::Lerp(oldPos.z, nextPos.z, kLerpRateZ);
	//見ている向き
	//カメラの角度
	m_angle = kCameraAngleX + (distance / 10000.0f);
	Vector3 dir = Matrix4x4::RotateXMat4x4(m_angle) * Vector3::Forward();
	//見てる位置
	Vector3 viewPos = nextPos + dir;
	//位置更新
	DxLib::SetCameraPositionAndTarget_UpVecY(nextPos.ToDxLibVector(), viewPos.ToDxLibVector());
	camera->SetPos(nextPos);
	camera->SetDir(dir);
	camera->SetViewPos(viewPos);
}
