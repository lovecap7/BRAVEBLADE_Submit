#pragma once
namespace Battle
{
	//攻撃の重さ
	enum class AttackWeight : int
	{
		Light		= 0,
		Middle		= 1,
		Heavy		= 2,
		Heaviest	= 3,
	};
	//攻撃でひるまない強さ
	typedef AttackWeight Armor;
	//演算子オーバーロード
	Battle::AttackWeight operator+(const Battle::AttackWeight& left, const Battle::AttackWeight& right);
	Battle::AttackWeight operator+(const Battle::AttackWeight& left, const int right);

	/// <summary>
	/// 攻撃とアーマーの比較
	/// 攻撃のほうが大きいならtrue
	/// </summary>
	/// <param name="aw"></param>
	/// <param name="am"></param>
	/// <returns></returns>
	bool CheckFlinchAttackAndArmor(AttackWeight aw, Armor am);
}
