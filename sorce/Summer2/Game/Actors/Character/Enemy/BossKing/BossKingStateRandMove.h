#pragma once
#include "BossKingStateBase.h"
class BossKingStateRandMove :
	public BossKingStateBase, public std::enable_shared_from_this<BossKingStateRandMove>
{
public:
	BossKingStateRandMove(std::weak_ptr<Actor> owner, bool isTransSecond);
	~BossKingStateRandMove();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//�ړ��t���[��
	int m_moveFrame;
	//�ړ��������
	Vector3 m_moveVec;
};

