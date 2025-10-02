#pragma once
#include "Battle.h"
#include <memory>
//体力やダメージを受けた時の処理などを行うクラス
class AttackBase;
class HitPoints
{
public:
	HitPoints();
	~HitPoints();
	//更新処理
	void Update();
	//初期化処理
	void ResetHitFlags();
	//体力
	void SetHp(int hp);
	int GetHp() { return m_hp; };
	int GetMaxHp() { return m_maxHp; };
	//回復
	void Heal(int heal);
	//ダメージ
	void Damage(int damage);
	//体力がなくなって死亡したかどうか
	bool IsDead()const { return m_hp <= 0; };
	//アーマー
	Battle::Armor GetArmor() { return m_defence.armor; };
	void SetArmor(Battle::Armor am);
	void AddArmor(int am);
	//攻撃を受けたか
	bool IsHit() { return m_isHit; };
	void SetIsHit(bool isHit) { m_isHit = isHit; };
	//ヒットリアクションをするか
	bool IsHitReaction() const { return m_isHitReaction; };
	void SetIsHitReaction(bool isHitReaction) { m_isHitReaction = isHitReaction; };
	//ダメージカット
	void SetDamageCutRate(float damageCutRate) { m_defence.damageCutRate = damageCutRate; };
	//無敵
	bool IsNoDamege() { return m_isNoDamage; };
	void SetIsNoDamege(bool isNoDamage) { m_isNoDamage = isNoDamage; };
	//アイテムをとった時によぶ関数
	void DefenseBuff(Battle::Armor armor, float damageCutRate, int buffFrame);
	//復活
	void Revival();
private:
	//体力
	int m_hp;
	//最大体力
	int m_maxHp;
	//攻撃を受けた
	bool m_isHit;
	//攻撃を受けたかどうか
	bool m_isHitReaction;
	//無敵
	bool m_isNoDamage;

	//防御面のステータス
	struct Defense
	{
		//アーマー
		Battle::Armor armor = Battle::Armor::Light;
		//ダメージカット率
		float damageCutRate = 1.0f;
	};
	Defense m_defence;
	Defense m_initDefence;	//バフ終了後の初期化用の値
	//ダメージカットアイテムをとった際にカウントするフレーム
	int m_buffCountFrame;
};

