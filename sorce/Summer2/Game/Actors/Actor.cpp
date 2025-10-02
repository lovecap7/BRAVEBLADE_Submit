#include "Actor.h"
#include "../../General/Rigidbody.h"
#include "Character/Enemy/EnemyBase.h"
#include "../../General/HitPoints.h"
#include "Attack/AttackBase.h"
#include "SearchPlace.h"
#include "../../General/Collision/Physics.h"

Actor::Actor(Shape shape):
	Collidable(shape),
	m_isDelete(false),
	m_id(0),
	m_isSetId(false),
	m_groupTag{ "Untagged" },
	m_canAttack(true),
	m_isInGroup(false),
	m_targetData{}
{
}

void Actor::SetID(int id)
{
	//すでに登録してるなら
	if (m_isSetId)return;
	m_id = id;
	m_isSetId = true;
}

Vector3 Actor::GetPos() const
{
	return m_rb->m_pos;
}

Vector3 Actor::GetNextPos() const
{
	return m_rb->GetNextPos();
}

void Actor::SetGroupTag(std::string& tag)
{
	m_groupTag = tag;
	//グループに所属しているか
	if (m_groupTag != "Untagged")
	{
		m_isInGroup = true;
	}
}

void Actor::TargetSearch(float searchDistance, float searchAngle, Vector3 targetPos)
{
	//リセット
	m_targetData.isHitTarget = false;
	//距離を確認
	auto myPos = m_rb->GetPos();
	auto dir = targetPos.XZ() - myPos.XZ();
	if (dir.Magnitude() <= searchDistance)
	{
		//視野角内にターゲットがいるか
		auto angle = abs(Vector2::GetRad(m_model->GetDir().XZ(), dir));
		if (angle <= (searchAngle / 2.0f))
		{
			m_targetData.isHitTarget = true;
			m_targetData.targetPos = targetPos;
			m_targetData.targetDirXZ = dir.XZ().Normalize();
			m_targetData.targetDis = dir.Magnitude();
		}
	}
}

void Actor::LookAtTarget()
{
	m_model->SetDir(m_targetData.targetDirXZ.XZ());
}

void Actor::OnHitFromAttack(const std::shared_ptr<Collidable> other)
{
	if (m_hitPoints->IsDead())return;//体力が0なら何もしない
	//自分と同じタグなら飛ばす
	if (std::dynamic_pointer_cast<AttackBase>(other)->GetOwnerTag() == m_tag)return;
	auto attack = std::dynamic_pointer_cast<AttackBase>(other);
	//攻撃を受けたのでフラグを立てる
	m_hitPoints->SetIsHit(true);
	//攻撃のダメージを受ける
	m_hitPoints->Damage(attack->GetDamage());
	//モデルの色とサイズを変更
	m_model->ModelHit();
	//ダメージを受けたら反応するかをチェック
	if (Battle::CheckFlinchAttackAndArmor(attack->GetAttackWeight(), m_hitPoints->GetArmor()))
	{
		m_hitPoints->SetIsHitReaction(true);				//反応する
		m_rb->AddVec(attack->GetKnockBackVec(m_rb->m_pos));	//ノックバック
	}
}

//探索場所があるかをチェックする関数
bool Actor::IsHaveSearchPlace() const
{
	//実態があるか
	if (m_searchPlace)
	{
		//範囲があるならtrue
		return m_searchPlace->GetRange() > 0.0f;
	}
	//ここまで来たら
	return false;
}

void Actor::SetSearchPlaceRang(float rang)
{
	//nullチェック
	if (m_searchPlace)
	{
		m_searchPlace->SetRange(rang);
	}
}
