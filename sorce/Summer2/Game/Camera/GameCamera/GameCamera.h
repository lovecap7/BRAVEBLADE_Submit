#pragma once
#include <memory>
#include "../../../General/Math/MyMath.h"
#include "../../../General/StageIndex.h"

enum class ShakePower : int
{
	None = 0,
	Low = 5,
	Middle = 10,
	High = 20,
	Highest = 40,
};

class ActorManager;
class Rigidbody;
class EventAreaBase;
class GameCameraStateBase;
class GameCamera:
	public std::enable_shared_from_this<GameCamera>
{
public:
	GameCamera();
	~GameCamera();
	//初期化処理
	void Init(Stage::StageIndex stageIndex);
	//更新処理
	void Update(const std::weak_ptr<ActorManager> actorManager);
	//カメラの座標
	Position3 GetPos() const { return m_pos; };
	void SetPos(Position3 pos) { m_pos = pos; };
	//カメラの向き
	Vector3 GetDir()const;
	void SetDir(Vector3 dir);
	//カメラの見ている位置
	Vector3 GetViewPos()const { return m_viewPos; };
	void SetViewPos(Vector3 viewPos) { m_viewPos = viewPos; };
	//イベントエリア
	std::weak_ptr<EventAreaBase> GetEventArea()const { return m_eventArea; };
	void SetEventArea(std::weak_ptr<EventAreaBase> event) { m_eventArea = event; };
	//カメラシェイク
	void UpdateCameraShake();
	void SetCameraShake(ShakePower power,int frame);
private:
	//自分の座標
	Position3 m_pos;
	//カメラの向き
	Vector3 m_dir;
	//カメラが見てる位置
	Vector3 m_viewPos;
	//イベントエリア
	std::weak_ptr<EventAreaBase> m_eventArea;
	//振動
	ShakePower m_shakePower;
	int m_shakeFrame;
	int m_maxShakeFrame;
	//状態遷移
	std::shared_ptr<GameCameraStateBase> m_state;
};

