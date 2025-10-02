#pragma once
#include "../CharacterBase.h"
#include "../../../../General/Math/MyMath.h"
#include <memory>
class Model;
class Collidable;
class ActorManager;
class Score;
class SelectStagePlayer :
	public CharacterBase
{
public:
	SelectStagePlayer(Vector3 cameraPos, Vector3 pos);
	~SelectStagePlayer();
	//初期化処理
	void Init() override;
	//更新処理
	void Update(Vector3 cameraPos,Vector3 targetPos);
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override {};
	//衝突イベント関数
	void OnCollide(const std::shared_ptr<Collidable> other)override {};
	//描画
	void Draw()const override;
	//更新処理による結果の確定
	void Complete() override {};
	//死亡処理
	void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override {};
	//終了処理
	void End()override;
private:
	//ダンスチェンジ
	bool m_isChangeDance;
	//決定
	bool m_isDecided;
	//サウンド
	void InitSound()override;
};
