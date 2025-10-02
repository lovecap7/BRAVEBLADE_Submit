#pragma once
#include "BossKingStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDataLoader.h"
#include "../../../../../General/Math/MyMath.h"

class GameCamera;
class Input;
class ActorManager;
class Actor;
class MyEffect;
class BossKingStateBlastAttack :
	public BossKingStateBase, public std::enable_shared_from_this<BossKingStateBlastAttack>
{
public:
	BossKingStateBlastAttack(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager);
	~BossKingStateBlastAttack();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//�U���̃t���[���𐔂���
	int m_attackCountFrame;
	//�U���̍쐬
	void CreateAttack(const std::weak_ptr<ActorManager> actorManager, Vector3 pos);
	//�U���f�[�^
	AttackData m_attackData;
	//���@�w�G�t�F�N�g
	std::weak_ptr<MyEffect> m_blastMagicEff;

private:
	//���@�w�̈ʒu�X�V
	void UpdateBlastMagicEffPos(Vector3 pos);
};


