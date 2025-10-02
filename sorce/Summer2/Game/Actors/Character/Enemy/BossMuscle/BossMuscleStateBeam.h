#pragma once
#include "BossMuscleStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDataLoader.h"

class GameCamera;
class Collidable;
class Model;
class Input;
class ActorManager;
class Actor;
class MyEffect;
class ULT;
class BossMuscle;
class BossMuscleStateBeam :
	public BossMuscleStateBase, public std::enable_shared_from_this<BossMuscleStateBeam>
{
public:
	BossMuscleStateBeam(std::weak_ptr<Actor> owner, bool isAngry, const std::weak_ptr<ActorManager> actorManager);
	~BossMuscleStateBeam();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//�U���̃t���[���𐔂���
	int m_attackCountFrame;
	//�U���ʒu�̍X�V
	void UpdateBeamPos(const Vector3 pos, std::shared_ptr<Model> model, std::shared_ptr<BossMuscle> coll);
    //�U���̍쐬
    void CreateAttack(VECTOR pos,const std::weak_ptr<ActorManager> actorManager);
    //�U���f�[�^
    AttackData m_attackData;
	//�r�[���̃`���[�W�G�t�F�N�g�Q��
	std::weak_ptr<MyEffect> m_beamChargeEff;
	//�r�[���̃G�t�F�N�g�Q��
	std::weak_ptr<MyEffect> m_beamEff;
	//�r�[���̍U���Q��
	std::weak_ptr<ULT> m_beam;
};