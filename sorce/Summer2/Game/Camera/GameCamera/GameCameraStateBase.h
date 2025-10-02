#pragma once
#include <memory>
#include "../../../General/Math/MyMath.h"
class GameCamera;
class ActorManager;
class GameCameraStateBase abstract
{
public:
	GameCameraStateBase(std::weak_ptr<GameCamera> camera);
	virtual ~GameCameraStateBase() {};
	//初期化処理
	virtual void Init()abstract;
	//状態に応じた更新処理
	virtual void Update(const std::weak_ptr<ActorManager> actorManager) abstract;
	//次の状態を取得
	std::shared_ptr<GameCameraStateBase> GetNextState() { return m_nextState; };
	//状態変化
	virtual void ChangeState(std::shared_ptr<GameCameraStateBase> nextState);
private:
	//カメラの状態
	std::shared_ptr<GameCameraStateBase> m_nextState;
protected:
	//カメラの持ち主
	std::weak_ptr<GameCamera> m_camera;
};

