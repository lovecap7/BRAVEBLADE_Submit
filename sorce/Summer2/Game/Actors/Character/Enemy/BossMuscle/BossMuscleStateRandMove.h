#pragma once
#include "BossMuscleStateBase.h"
class BossMuscleStateRandMove :
	public BossMuscleStateBase, public std::enable_shared_from_this<BossMuscleStateRandMove>
{
public:
	BossMuscleStateRandMove(std::weak_ptr<Actor> owner, bool isAngry);
	~BossMuscleStateRandMove();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//�ړ��t���[��
	int m_moveFrame;
	//�ړ��������
	Vector3 m_moveVec;
};

