#include "GroupManager.h"
#include "Actor.h"
#include "../../General/Model.h"

GroupManager::GroupManager()
{
}

GroupManager::~GroupManager()
{
}

void GroupManager::Entry(std::shared_ptr<Actor> actor)
{
	std::string groupName = actor->GetGroupTag();
	//アクターを追加
	m_groupActors[groupName].emplace_back(actor);
	//まだ登録されていないグループ名なら
	auto it = std::find(m_groupNames.begin(), m_groupNames.end(), groupName);
	if (it == m_groupNames.end())
	{
		//グループ名登録
		m_groupNames.emplace_back(groupName);
		//攻撃権を持たせる
		TakeTurnCanAttack(actor, groupName);
#if _DEBUG
		m_groupColor[groupName] = GetColor(MyMath::GetRand(0, 255), MyMath::GetRand(0, 255), MyMath::GetRand(0, 255));
#endif
	}
	else
	{
		//攻撃権をなくす
		actor->SetCanAttack(false);
	}
}

void GroupManager::Exit(std::shared_ptr<Actor> actor)
{
	std::string groupName = actor->GetGroupTag();

	//登録されているなら
	auto it = std::find(m_groupActors[groupName].begin(), m_groupActors[groupName].end(), actor);
	if (it == m_groupActors[groupName].end())return;

	//もし攻撃権を持っているなら
	if (actor->CanAttack())
	{
		auto nextIt = it;
		++nextIt;
		//次のアクターに渡す
		if (nextIt != m_groupActors[groupName].end())
		{
			//攻撃権を持たせる
			TakeTurnCanAttack(*nextIt, groupName);
		}
		else
		{
			//次のアクターがいないなら先頭に渡す
			TakeTurnCanAttack(m_groupActors[groupName].front(), groupName);
		}
	}

	//登録解除
	m_groupActors[groupName].remove(actor);
}

void GroupManager::Init()
{
	m_groupActors = {};
	m_canAttackActorID = {};
	m_groupNames = {};
}

void GroupManager::Update()
{
	//登録されたグループ名から各グループを見る
	for (auto& groupName : m_groupNames)
	{
		//もしもグループ内で誰もターゲットを発見していない場合は
		//回す必要はないので処理をしない
		bool isGroupSearch = false;
		for (auto& actor : m_groupActors[groupName])
		{
			//発見できているなら
			if (actor->IsSearch())
			{
				isGroupSearch = true;
				break;
			}
		}
		if (!isGroupSearch)continue;

		//攻撃権を次に回すかどうか
		bool isTurnCanAttack = false;
		//回せたか
		bool isTurnedCanAttack = false;
		//グループ内で攻撃権を回す
		for (auto& actor : m_groupActors[groupName])
		{
#if _DEBUG
			//足元に球体を出して色からどのグループに所属しているかわかるようにする
			DrawSphere3D(actor->GetPos().ToDxLibVector(), 40.0f, 16, m_groupColor[groupName], m_groupColor[groupName], true);
#endif
			//アクターのID
			int actorId = actor->GetID();
			//攻撃権を持つアクターのID
			int canAttackId = m_canAttackActorID[groupName];
			//攻撃権があるか
			bool canAttack = actor->CanAttack();
			//もしもプレイヤーを発見できていない敵キャラクターに攻撃権が移った場合
			//そのキャラが攻撃権を保持し続けるので発見してない場合は譲る
			if (actorId == canAttackId && canAttack && !actor->IsSearch())
			{
				//攻撃権がなくなる
				actor->SetCanAttack(false);
			}
			//回さない
			if (!isTurnCanAttack)
			{
				//IDから攻撃権所有者かつ攻撃権がなくなっているなら
				if (actorId == canAttackId && !canAttack)
				{
					//次のアクターに渡す
					isTurnCanAttack = true;
				}
			}
			//回す
			else
			{
				//渡せた
				isTurnedCanAttack = true;
				TakeTurnCanAttack(actor, groupName);
				break;
			}
		}
		//渡すつもりだったが次のアクターがいなくて渡せなかったとき
		if (isTurnCanAttack && !isTurnedCanAttack)
		{
			//先頭に渡す
			TakeTurnCanAttack(m_groupActors[groupName].front(), groupName);
		}
	}
}

void GroupManager::End()
{
	//アクターの登録解除
	std::list<std::shared_ptr<Actor>> deleteActer;
	for (auto& groupName : m_groupNames)
	{
		for (auto& actor : m_groupActors[groupName])
		{
			deleteActer.emplace_back(actor);
		}
	}
	for (auto& actor : deleteActer)
	{
		Exit(actor);
	}
	deleteActer.clear();
	//すべて登録解除
	m_groupActors.clear();
	m_canAttackActorID.clear();
}

void GroupManager::TakeTurnCanAttack(std::shared_ptr<Actor> actor, std::string& groupName)
{
	//攻撃権を持たせる
	actor->SetCanAttack(true);
	//ID登録
	m_canAttackActorID[groupName] = actor->GetID();
}
