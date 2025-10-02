#include "Battle.h"
#include "Math/MathSub.h"

Battle::AttackWeight Battle::operator+(const Battle::AttackWeight& left, const Battle::AttackWeight& right)
{
	int result = static_cast<int>(left) + static_cast<int>(right);
	result = MathSub::ClampInt(result, static_cast<int>(Battle::AttackWeight::Light), static_cast<int>(Battle::AttackWeight::Heaviest));
	return  static_cast<Battle::AttackWeight>(result);
}

Battle::AttackWeight Battle::operator+(const Battle::AttackWeight& left, const int right)
{
	int result = static_cast<int>(left) + right;
	result = MathSub::ClampInt(result, static_cast<int>(Battle::AttackWeight::Light), static_cast<int>(Battle::AttackWeight::Heaviest));
	return  static_cast<Battle::AttackWeight>(result);
}

bool Battle::CheckFlinchAttackAndArmor(AttackWeight ap, Armor am)
{
	//攻撃の大きさがアーマーより大きいならtrue
	return (ap > am);
}
