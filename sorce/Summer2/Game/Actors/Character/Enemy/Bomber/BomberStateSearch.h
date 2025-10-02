#pragma once
#include "BomberStateBase.h"
class GameCamera;
class ActorManager;
class Actor;
class BomberStateSearch :
	public BomberStateBase, public std::enable_shared_from_this<BomberStateSearch>
{
public:
	BomberStateSearch(std::weak_ptr<Actor> owner);
	~BomberStateSearch();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//�ړ��t���[��
	int m_moveFrame;
	//�ړ��������
	Vector3 m_moveVec;
};

