#pragma once
#include "ItemBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class GameCamera;
class Bomb :
	public ItemBase
{
public:
	Bomb(int modelHandle, Vector3 pos);
	virtual ~Bomb();
	//初期化処理
	void Init()override;
	//更新処理
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//衝突イベント
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//死亡処理
	void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override;
	//移動量
	void SetVec(Vector3 vec);
private:
	//爆発のカウント
	int m_blastCountFrame;
	//モデルのサイズ
	Vector3 m_originalScale;//元のモデルのサイズを保持
	//モデルの大きさをcosを使って変更するので角度を保持
	float m_scaleSpeed;
	//爆弾のアニメーション
	void AnimBomb();
};
