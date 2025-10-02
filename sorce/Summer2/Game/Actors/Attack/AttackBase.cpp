#include "AttackBase.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../Character/CharacterStateBase.h"
#include "../Character/Player/UltGage.h"
#include "../../../General/HitPoints.h"
#include "../../../General/AttackPoints.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Rigidbody.h"
#include "../ActorManager.h"

AttackBase::AttackBase(Shape shape, std::weak_ptr<Actor> owner) :
	Actor(shape),
	m_owner(owner),
	m_damage(0),
	m_keepFrame(0),
	m_knockBackPower(0.0f),
	m_attackWeight(Battle::AttackWeight::Light),
	m_hitStopFrame(0),
	m_shakePower(ShakePower::None),
	m_ownerTag(owner.lock()->GetGameTag()),
	m_isHit(false),
	m_isSuccessAttack(false)
{
}

AttackBase::~AttackBase()
{
}

void AttackBase::Init()
{
	AllSetting(CollisionState::Normal, Priority::Low, GameTag::Attack, false, true,false);
	Collidable::Init();
}

void AttackBase::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//攻撃が当たったなら
	if (m_isHit)
	{
		//ヒットストップ
		actorManager.lock()->HitStop(m_shakePower, m_hitStopFrame);
	}
	//持ち主の参照が切れたら
	if (m_owner.expired())
	{
		m_isDelete = true; //所有者のステートが無くなったら削除フラグを立てる
		return; //所有者のステートが無くなったら何もしない
	}
	//持続フレームを減らす
	--m_keepFrame;
	//持続フレームが0になったら削除
	if (m_keepFrame < 0)
	{
		m_isDelete = true;	//削除フラグを立てる
		m_isThrough = true;	//当たり判定をしない
		return; //何もしない
	}
	//初期化
	m_isHit = false;
}

void AttackBase::OnCollide(const std::shared_ptr<Collidable> other)
{
	if (m_owner.expired())return;
	m_isSuccessAttack = false;
	auto ownerColl = m_owner.lock();
	auto otherColl = other;

	//自分と同じ種類なら無視
	if (otherColl->GetGameTag() == ownerColl->GetGameTag())return;

	//プレイヤーか敵なら
	if (otherColl->GetGameTag() == GameTag::Player ||
		otherColl->GetGameTag() == GameTag::Enemy)
	{
		if (std::dynamic_pointer_cast<CharacterBase>(otherColl)->GetHitPoints().lock()->IsNoDamege())
		{
			//ダメージを受けない状態なら無視
			return;
		}
	}
	else
	{
		//それ以外のActorは無視
		return;
	}
	std::shared_ptr<Actor> otherActor = std::dynamic_pointer_cast<Actor>(otherColl);
	bool isFind = false;
	//IDがすでに記録されているか確認
	for (auto id : m_hitId)
	{
		if (id == otherActor->GetID())
		{
			isFind = true;
			break;
		}
	}
	//攻撃成功
	if (!isFind)
	{
		//記録されていなければ記録する
		m_hitId.emplace_back(otherActor->GetID());
		//プレイヤーの攻撃の場合必殺技ゲージを加算する
		if (ownerColl->GetGameTag() == GameTag::Player)
		{
			std::dynamic_pointer_cast<Player>(m_owner.lock())->GetUltGage().lock()->AddPedingUltGage();//予約されていた加算ゲージ量を反映
		}
		//攻撃を受けたときの処理
		std::dynamic_pointer_cast<CharacterBase>(otherColl)->OnHitFromAttack(shared_from_this());
		//攻撃に成功したので
		m_isSuccessAttack = true;
		//攻撃を当てたので
		m_isHit = true;
	}
}

void AttackBase::End()
{
	Collidable::End();
}

Vector3 AttackBase::GetKnockBackVec(Position3 other)
{
	//ノックバック
	Vector3 knockBackVec = other - m_owner.lock()->GetNextPos();
	knockBackVec.y = 0.0f;//Y成分はなし
	knockBackVec = knockBackVec.Normalize() * m_knockBackPower;//ノックバック
	return knockBackVec;
}

void AttackBase::AttackSetting(int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, int hitStopFrame, ShakePower sp)
{
	m_damage = damage;
	m_keepFrame = keepFrame;
	m_knockBackPower = knockBackPower;
	m_attackWeight = aw;
	m_hitStopFrame = hitStopFrame;
	m_shakePower = sp;
}

int AttackBase::GetDamage()
{
	float damage = static_cast<float>(m_damage);
	if (!m_owner.expired())
	{
		auto owner = m_owner.lock();
		if(owner->GetAttackPoints().expired())return static_cast<int>(damage);
		std::shared_ptr<AttackPoints> attackPoints = owner->GetAttackPoints().lock();
		//攻撃力
		damage += attackPoints->GetAttackPower();
		//ダメージ倍率をかける
		damage *= attackPoints->GetDamageRate();
	}
	return static_cast<int>(damage);
}

Battle::AttackWeight AttackBase::GetAttackWeight()
{
	Battle::AttackWeight aw = m_attackWeight;
	if (!m_owner.expired())
	{
		//攻撃の重さが最低値未満かどうか
		auto owner = m_owner.lock();
		if (owner->GetGameTag() == GameTag::Player ||
			owner->GetGameTag() == GameTag::Enemy)
		{
			//持ち主の最低値
			auto ownerAw = std::dynamic_pointer_cast<CharacterBase>(owner)->GetAttackPoints().lock()->GetLowestAW();
			//持ち主のアーマーより小さいなら
			if (!Battle::CheckFlinchAttackAndArmor(aw, ownerAw))
			{
				aw = ownerAw;
			}
		}
	}
	return aw;
}
