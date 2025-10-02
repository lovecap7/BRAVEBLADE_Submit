#pragma once
#include "SmallDragonStateBase.h"
class GameCamera;
class ActorManager;
class Actor;
class SmallDragonStateSearch :
	public SmallDragonStateBase, public std::enable_shared_from_this<SmallDragonStateSearch>
{
public:
	SmallDragonStateSearch(std::weak_ptr<Actor> owner);
	~SmallDragonStateSearch();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//�ړ��t���[��
	int m_moveFrame;
	//�ړ��������
	Vector3 m_moveVec;
};

