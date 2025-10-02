#pragma once
#include <map>
#include <list>
#include <string>
#include <memory>
#include <DxLib.h>
class Actor;
//グループ内での処理を行うクラス
class GroupManager
{
public:
	GroupManager();
	~GroupManager();
	void Entry(std::shared_ptr<Actor> actor);
	void Exit(std::shared_ptr<Actor> actor);
	void Init();
	void Update();
	void End();
private:
	//各グループのアクター
	std::map<std::string, std::list<std::shared_ptr<Actor>>> m_groupActors;
	//各グループの攻撃権を持つアクターのID
	std::map<std::string, int> m_canAttackActorID;
	//各グループ名
	std::list<std::string> m_groupNames;
	//攻撃の権利を回す
	void TakeTurnCanAttack(std::shared_ptr<Actor> actor, std::string& groupName);
#if _DEBUG
	//デバッグ用
	//グループカラー
	std::map<std::string, unsigned int> m_groupColor;
#endif
};

