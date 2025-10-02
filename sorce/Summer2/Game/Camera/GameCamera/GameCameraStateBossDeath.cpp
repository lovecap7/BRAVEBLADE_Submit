#include "GameCameraStateBossDeath.h"
#include "GameCameraStateClear.h"
#include "GameCameraStateNormal.h"
#include "GameCameraStateBossArea.h"
#include "GameCamera.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/HitPoints.h"
#include "../../../General/game.h"
#include "../../Actors/Character/Player/Player.h"
#include "../../Actors/Character/Enemy/EnemyBase.h"
#include "../../Actors/ActorManager.h"
#include "../../../General/Collision/Physics.h"
#include "../../../General/Sound/SoundManager.h"
#include <DxLib.h>

namespace
{
	//視野角
	constexpr float kPerspective = 35.0f * MyMath::DEG_2_RAD;
	//カメラ角度
	constexpr float kCameraAngleX = 30.0f * MyMath::DEG_2_RAD;
	//lerpの割合
	constexpr float kLerpRateX = 0.05f;
	constexpr float kLerpRateY = 0.05f;
	constexpr float kLerpRateZ = 0.01f;
	//ターゲットから少し離れるためのオフセット
	constexpr float kOffsetCameraPosY = 700.0f;
	constexpr float kOffsetCameraPosZ = -1300.0f;
	//壁からの距離
	constexpr float kDistanceFromWall = 300.0f;
	//ヒットストップのフレーム
	constexpr int kHitStopFrame = 60;
	//揺れるフレーム
	constexpr int kShakeFrame = 30;
	//nearとfar
	constexpr float kNear = 300.0f;
	constexpr float kFar = 20000.0f;
}
GameCameraStateBossDeath::GameCameraStateBossDeath(std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) :
	GameCameraStateBase(camera)
{
	//カメラがあるかチェック
	if (m_camera.expired())return;
	auto owner = m_camera.lock();
	auto boss = actorManager.lock()->GetBoss().lock();
	//位置更新
	DxLib::SetCameraPositionAndTarget_UpVecY(owner->GetPos().ToDxLibVector(), boss->GetPos().ToDxLibVector());
	//撃破ストップ
	actorManager.lock()->DelayUpdate(kHitStopFrame);
	owner->SetCameraShake(ShakePower::Highest, kShakeFrame);
	Physics::GetInstance().DelayUpdate(60);
	SoundManager::GetInstance().PlayOnceSE("LastHit");
	//奥行
	SetCameraNearFar(kNear, kFar);
}

void GameCameraStateBossDeath::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void GameCameraStateBossDeath::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//ボスが消滅したらゲームクリアカメラに
	if (actorManager.lock()->IsBossDisappear())
	{
		ChangeState(std::make_shared<GameCameraStateClear>(m_camera, actorManager));
		return;
	}
	//ボスが存在しない場合は何もしない
	if (actorManager.lock()->GetBoss().expired())return;
	auto boss = actorManager.lock()->GetBoss().lock();
	//死んでいないなら
	if (!boss->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<GameCameraStateBossArea>(m_camera));
		return;
	}
	//カメラがあるかチェック
	if (m_camera.expired())return;
	auto camera = m_camera.lock();
	//位置更新
	DxLib::SetCameraPositionAndTarget_UpVecY(camera->GetPos().ToDxLibVector(), boss->GetPos().ToDxLibVector());
}
