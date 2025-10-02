#pragma once
#include <map>
#include <list>
#include <string>
#include <memory>
#include <DxLib.h>
class Actor;
//�O���[�v���ł̏������s���N���X
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
	//�e�O���[�v�̃A�N�^�[
	std::map<std::string, std::list<std::shared_ptr<Actor>>> m_groupActors;
	//�e�O���[�v�̍U���������A�N�^�[��ID
	std::map<std::string, int> m_canAttackActorID;
	//�e�O���[�v��
	std::list<std::string> m_groupNames;
	//�U���̌�������
	void TakeTurnCanAttack(std::shared_ptr<Actor> actor, std::string& groupName);
#if _DEBUG
	//�f�o�b�O�p
	//�O���[�v�J���[
	std::map<std::string, unsigned int> m_groupColor;
#endif
};

