#pragma once
#include "BossMuscleStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDataLoader.h"

class GameCamera;
class AreaOfEffectAttack;
class Input;
class ActorManager;
class Actor;
class BossMuscleStateSwingAttack :
	public BossMuscleStateBase, public std::enable_shared_from_this<BossMuscleStateSwingAttack>
{
public:
	BossMuscleStateSwingAttack(std::weak_ptr<Actor> owner, bool isAngry, const std::weak_ptr<ActorManager> actorManager);
	~BossMuscleStateSwingAttack();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//�U���̃t���[���𐔂���
	int m_attackCountFrame;
	//�U���̎Q��
	std::weak_ptr<AreaOfEffectAttack> m_attack;
	//�U���̍쐬
	void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
	//�U���̈ʒu���X�V����
	void UpdateAttackPos();
	//�U�����ɑO�i����
	void AttackMove();
	//�U���f�[�^
	AttackData m_attackData;
};