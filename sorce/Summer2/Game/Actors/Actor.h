#pragma once
#include <memory>
#include <string>
#include "../../General/Math/MyMath.h"
#include "../../General/Collision/Collidable.h"

class ActorManager;
class GameCamera;
class Collidable;
class Score;
class HitPoints;
class AttackPoints;
class SearchPlace;
class Model;
/// <summary>
/// ゲーム中に配置可能な物体の基底クラス
/// </summary>
/// //newできなくなる
class Actor abstract :
	public Collidable
{
public:
	//ターゲットに関するデータ構造体
	struct TargetData
	{
		//ターゲットを発見
		bool isHitTarget = false;
		//ターゲットの座標
		Position3 targetPos = Vector3::Zero();
		//ターゲットへの向き
		Vector3 targetDirXZ = Vector3::Zero();
		//ターゲットとの距離
		float targetDis = 0.0f;
	};
protected:
	//消滅
	bool m_isDelete;
	//アクターの識別番号
	int m_id;
	//IDがセットされたことを記録するフラグ
	bool m_isSetId;
	//モデル
	std::shared_ptr<Model> m_model;
	//グループ
	std::string m_groupTag;
	//攻撃権
	bool m_canAttack;
	//グループに所属しているか
	bool m_isInGroup;
	//ターゲットの情報
	TargetData m_targetData;
	//体力
	std::shared_ptr<HitPoints> m_hitPoints;
	//攻撃のステータス
	std::shared_ptr<AttackPoints> m_attackPoints;
	//探索場所
	std::shared_ptr<SearchPlace> m_searchPlace;
public:
	Actor(Shape shape);
	virtual ~Actor() {};
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init()abstract;
	/// <summary>
	/// 位置などの更新を行う(引数なし)
	/// </summary>
	virtual void Update() {};
	/// <summary>
	/// 位置などの更新を行う
	/// </summary>
	virtual void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) abstract;
	/// <summary>
	/// 対象のアクターを表示
	/// </summary>
	virtual void Draw()const abstract;
	/// <summary>
	/// 死亡した際の処理
	/// </summary>
	/// <param name="actorManager"></param>
	virtual void Dead(const std::weak_ptr<ActorManager> actorManager,const std::weak_ptr<Score> score)abstract;
	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void End()abstract;
	/// <summary>
	/// 削除フラグ
	/// </summary>
	/// <returns></returns>
	bool IsDelete() { return m_isDelete; };
	/// <summary>
	/// 削除予約
	/// </summary>
	/// <returns></returns>
	void Delete() { m_isDelete = true; };
	/// <summary>
	/// IDを取得
	/// </summary>
	/// <returns></returns>
	int GetID() const { return m_id; };
	/// <summary>
	/// IDをセット
	/// </summary>
	/// <returns></returns>
	void SetID(int id);
	/// <summary>
	/// 座標
	/// </summary>
	Vector3 GetPos() const;
	/// <summary>
	/// 次の座標
	/// </summary>
	Vector3 GetNextPos() const;
	//モデルクラス
	std::shared_ptr<Model> GetModel() const { return m_model; };
	//グループ
	std::string GetGroupTag()const { return m_groupTag; };
	void SetGroupTag(std::string& tag);
	//グループに所属しているか
	bool IsInGroup()const { return m_isInGroup; };
	//攻撃権
	bool CanAttack()const { return m_canAttack; };
	void SetCanAttack(bool canAttack) { m_canAttack = canAttack; };
	//ターゲットの索敵
	virtual void TargetSearch(float searchDistance, float searchAngle, Vector3 targetPos);
	//ターゲットを見る
	void LookAtTarget();
	//ターゲットを発見できたか
	bool IsSearch()const { return m_targetData.isHitTarget; };
	//体力のステータス
	std::weak_ptr<HitPoints> GetHitPoints() const { return m_hitPoints; };
	//攻撃のステータス
	std::weak_ptr<AttackPoints> GetAttackPoints() const { return m_attackPoints; };
	//攻撃を受けたときの処理
	virtual void OnHitFromAttack(const std::shared_ptr<Collidable> other);
	//探索場所の参照
	std::weak_ptr<SearchPlace> GetSearchPlace() { return m_searchPlace; };
	//探索場所があるか
	bool IsHaveSearchPlace()const;
	//探索範囲
	void SetSearchPlaceRang(float rang);
};

