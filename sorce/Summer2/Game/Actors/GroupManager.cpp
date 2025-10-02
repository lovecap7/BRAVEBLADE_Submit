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
	//�A�N�^�[��ǉ�
	m_groupActors[groupName].emplace_back(actor);
	//�܂��o�^����Ă��Ȃ��O���[�v���Ȃ�
	auto it = std::find(m_groupNames.begin(), m_groupNames.end(), groupName);
	if (it == m_groupNames.end())
	{
		//�O���[�v���o�^
		m_groupNames.emplace_back(groupName);
		//�U��������������
		TakeTurnCanAttack(actor, groupName);
#if _DEBUG
		m_groupColor[groupName] = GetColor(MyMath::GetRand(0, 255), MyMath::GetRand(0, 255), MyMath::GetRand(0, 255));
#endif
	}
	else
	{
		//�U�������Ȃ���
		actor->SetCanAttack(false);
	}
}

void GroupManager::Exit(std::shared_ptr<Actor> actor)
{
	std::string groupName = actor->GetGroupTag();

	//�o�^����Ă���Ȃ�
	auto it = std::find(m_groupActors[groupName].begin(), m_groupActors[groupName].end(), actor);
	if (it == m_groupActors[groupName].end())return;

	//�����U�����������Ă���Ȃ�
	if (actor->CanAttack())
	{
		auto nextIt = it;
		++nextIt;
		//���̃A�N�^�[�ɓn��
		if (nextIt != m_groupActors[groupName].end())
		{
			//�U��������������
			TakeTurnCanAttack(*nextIt, groupName);
		}
		else
		{
			//���̃A�N�^�[�����Ȃ��Ȃ�擪�ɓn��
			TakeTurnCanAttack(m_groupActors[groupName].front(), groupName);
		}
	}

	//�o�^����
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
	//�o�^���ꂽ�O���[�v������e�O���[�v������
	for (auto& groupName : m_groupNames)
	{
		//�������O���[�v���ŒN���^�[�Q�b�g�𔭌����Ă��Ȃ��ꍇ��
		//�񂷕K�v�͂Ȃ��̂ŏ��������Ȃ�
		bool isGroupSearch = false;
		for (auto& actor : m_groupActors[groupName])
		{
			//�����ł��Ă���Ȃ�
			if (actor->IsSearch())
			{
				isGroupSearch = true;
				break;
			}
		}
		if (!isGroupSearch)continue;

		//�U���������ɉ񂷂��ǂ���
		bool isTurnCanAttack = false;
		//�񂹂���
		bool isTurnedCanAttack = false;
		//�O���[�v���ōU��������
		for (auto& actor : m_groupActors[groupName])
		{
#if _DEBUG
			//�����ɋ��̂��o���ĐF����ǂ̃O���[�v�ɏ������Ă��邩�킩��悤�ɂ���
			DrawSphere3D(actor->GetPos().ToDxLibVector(), 40.0f, 16, m_groupColor[groupName], m_groupColor[groupName], true);
#endif
			//�A�N�^�[��ID
			int actorId = actor->GetID();
			//�U���������A�N�^�[��ID
			int canAttackId = m_canAttackActorID[groupName];
			//�U���������邩
			bool canAttack = actor->CanAttack();
			//�������v���C���[�𔭌��ł��Ă��Ȃ��G�L�����N�^�[�ɍU�������ڂ����ꍇ
			//���̃L�������U������ێ���������̂Ŕ������ĂȂ��ꍇ�͏���
			if (actorId == canAttackId && canAttack && !actor->IsSearch())
			{
				//�U�������Ȃ��Ȃ�
				actor->SetCanAttack(false);
			}
			//�񂳂Ȃ�
			if (!isTurnCanAttack)
			{
				//ID����U�������L�҂��U�������Ȃ��Ȃ��Ă���Ȃ�
				if (actorId == canAttackId && !canAttack)
				{
					//���̃A�N�^�[�ɓn��
					isTurnCanAttack = true;
				}
			}
			//��
			else
			{
				//�n����
				isTurnedCanAttack = true;
				TakeTurnCanAttack(actor, groupName);
				break;
			}
		}
		//�n�����肾���������̃A�N�^�[�����Ȃ��ēn���Ȃ������Ƃ�
		if (isTurnCanAttack && !isTurnedCanAttack)
		{
			//�擪�ɓn��
			TakeTurnCanAttack(m_groupActors[groupName].front(), groupName);
		}
	}
}

void GroupManager::End()
{
	//�A�N�^�[�̓o�^����
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
	//���ׂēo�^����
	m_groupActors.clear();
	m_canAttackActorID.clear();
}

void GroupManager::TakeTurnCanAttack(std::shared_ptr<Actor> actor, std::string& groupName)
{
	//�U��������������
	actor->SetCanAttack(true);
	//ID�o�^
	m_canAttackActorID[groupName] = actor->GetID();
}
