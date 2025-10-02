#pragma once
#include "PurpleDinosaurStateBase.h"
class GameCamera;
class ActorManager;
class Actor;
class PurpleDinosaurStateSearch :
    public PurpleDinosaurStateBase, public std::enable_shared_from_this<PurpleDinosaurStateSearch>
{
public:
	PurpleDinosaurStateSearch(std::weak_ptr<Actor> owner);
    ~PurpleDinosaurStateSearch();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//�ړ��t���[��
	int m_moveFrame;
	//�ړ��������
	Vector3 m_moveVec;
};

