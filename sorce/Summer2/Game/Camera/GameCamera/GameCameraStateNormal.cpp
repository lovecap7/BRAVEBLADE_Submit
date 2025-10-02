#include "GameCamera.h"
#include "GameCameraStateNormal.h"
#include "GameCameraStateAreaLock.h"
#include "GameCameraStateZMove.h"
#include "GameCameraStateBossArea.h"
#include "GameCameraStateBossDeath.h"
#include "GameCameraStateClear.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/HitPoints.h"
#include "../../../General/game.h"
#include "../../Actors/Character/Player/Player.h"
#include "../../Actors/Character/Enemy/EnemyBase.h"
#include "../../Actors/Stage/BossArea.h"
#include "../../Actors/ActorManager.h"
#include "../../Actors/Stage/EventAreaBase.h"
#include <DxLib.h>

namespace
{
	//視野角
	constexpr float kPerspective = 35.0f * MyMath::DEG_2_RAD;
	//カメラ角度
	constexpr float kCameraAngleX = 30.0f * MyMath::DEG_2_RAD;
	//ターゲットから少し離れるためのオフセット
	constexpr float kOffsetCameraPosY = 800.0f;
	constexpr float kCameraPosZ = -900.0f;
	//カメラをずらす距離の上限
	constexpr float kSlideMax = 200.0f;
	constexpr float kSlideMin = -200.0f;
	//カメラのスライド速度率
	constexpr float kSlideSpeedRate = 0.4f;
	//移動開始フレーム
	constexpr int kMoveStartFrame = 60;
	//画面中央に戻す処理を行うのにかかるフレーム
	constexpr int kMoveEndFrame = 120;
	//中央に戻すlerp率
	constexpr float kSlideCenterRate = 0.05f;
	//nearとfar
	constexpr float kNear = 300.0f;
	constexpr float kFar = 20000.0f;
}

GameCameraStateNormal::GameCameraStateNormal(std::weak_ptr<GameCamera> camera):
	GameCameraStateBase(camera),
	m_cameraSlide(0.0f),
	m_moveStartFrame(0),
	m_moveEndFrame(0)
{
	//カメラがあるかチェック
	if (m_camera.expired())return;
	auto owner = m_camera.lock();
	//カメラの角度
	owner->SetDir(Matrix4x4::RotateXMat4x4(kCameraAngleX) *
		Vector3::Forward());
	//カメラの座標と注視点
	SetCameraPositionAndTarget_UpVecY(owner->GetPos().ToDxLibVector(), owner->GetViewPos().ToDxLibVector());
	//視野角
	SetupCamera_Perspective(kPerspective);
	//奥行
	SetCameraNearFar(kNear, kFar);
}

void GameCameraStateNormal::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void GameCameraStateNormal::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//ボスが死亡した場合
	if (actorManager.lock()->IsBossDead())
	{
		ChangeState(std::make_shared<GameCameraStateBossDeath>(m_camera, actorManager));
		return;
	}
	//プレイヤーが消滅した場合更新終了
	if (actorManager.lock()->GetPlayer().expired())return;
	auto player = actorManager.lock()->GetPlayer().lock();
	auto camera = m_camera.lock();
	//イベントエリアにプレイヤーが入ったなら
	if (!camera->GetEventArea().expired())
	{
		if (camera->GetEventArea().lock()->IsEvent())
		{
			//入ったエリアに応じて状態変化
			auto camera = m_camera.lock();
			switch (camera->GetEventArea().lock()->GetAreaTag())
			{
			case AreaTag::AllKill:
				//全滅エリア
				ChangeState(std::make_shared<GameCameraStateAreaLock>(m_camera));
				return;
				//Z軸移動エリア
			case AreaTag::ZMove:
				ChangeState(std::make_shared<GameCameraStateZMove>(m_camera));
				return;
			case AreaTag::Boss:
				//ボスエリア
				ChangeState(std::make_shared<GameCameraStateBossArea>(m_camera));
				return;
			}
		}
	}
	//移動
	auto playerPos = player->GetRb()->GetPos();
	//位置の更新
	Vector3 oldPos = camera->GetPos();
	Vector3 nextPos = camera->GetPos();
	nextPos.z = kCameraPosZ;
	nextPos.y = playerPos.y + kOffsetCameraPosY;//プレイヤーのY座標より高い位置
	//滑らかに
	nextPos = Vector3::Lerp(oldPos, nextPos, 0.05f);

	//移動しているなら
	if (player->GetRb()->GetVec().x != 0.0f && player->GetCollState() == CollisionState::Move)
	{
		//移動しているフレームの加算
		++m_moveStartFrame;
		m_moveStartFrame = MathSub::ClampInt(m_moveStartFrame, 0, kMoveStartFrame);
		//一定フレーム移動しているならスライド開始
		if (m_moveStartFrame >= kMoveStartFrame)
		{
			//カメラをプレイヤーの移動量の半分くらいの大きさで移動
			m_cameraSlide += player->GetRb()->GetVec().x * kSlideSpeedRate;
		}
		//移動しているので移動していないフレームはカウントしない
		m_moveEndFrame = 0;
	}
	//移動していない
	else
	{
		//移動をしていないフレームを数える
		++m_moveEndFrame;
		m_moveEndFrame = MathSub::ClampInt(m_moveEndFrame, 0, kMoveEndFrame);
		//移動していないフレームが一定フレーム経過したら
		if (m_moveEndFrame >= kMoveEndFrame)
		{
			//画面中央に戻す
			m_cameraSlide = MathSub::Lerp(m_cameraSlide, 0.0f, kSlideCenterRate);
			//移動していないので
			m_moveStartFrame = 0;
		}
	}
	//範囲内に収める(下限 ~ 上限の間)
	m_cameraSlide = MathSub::ClampFloat(m_cameraSlide, kSlideMin, kSlideMax);
	//プレイヤーのX座標からm_cameraSlideだけ動かす
	nextPos.x = playerPos.x + m_cameraSlide;

	//見ている向き
	Vector3 dir = camera->GetDir();
	//見てる位置
	Vector3 viewPos = camera->GetViewPos();
	viewPos = nextPos + dir;
	//位置更新
	SetCameraPositionAndTarget_UpVecY(nextPos.ToDxLibVector(), viewPos.ToDxLibVector());
	camera->SetPos(nextPos);
	camera->SetDir(dir);
	camera->SetViewPos(viewPos);
}