#pragma once
#include "BossKingStateBase.h"
#include "../../../../../General/Battle.h"
#include "../../../../../General/CSVDatas.h"
#include "../../../../../General/Math/MyMath.h"

class GameCamera;
class Input;
class ActorManager;
class Actor;
class BossKing;
class Model;
class MyEffect;
class ULT;
class BossKingStateBeam :
	public BossKingStateBase, public std::enable_shared_from_this<BossKingStateBeam>
{
public:
	BossKingStateBeam(std::weak_ptr<Actor> owner, bool isTransSecond,const std::weak_ptr<ActorManager> actorManager);
	~BossKingStateBeam();
	void Init()override;
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//�U���̃t���[���𐔂���
	int m_attackCountFrame;
	//�U���ʒu�̍X�V
	void UpdateBeamPos(const Vector3 pos, std::shared_ptr<Model> model, std::shared_ptr<BossKing> coll);
	//�U���̍쐬
	void CreateAttack(VECTOR pos, const std::weak_ptr<ActorManager> actorManager);
	//�U���f�[�^
	AttackData m_attackData;
	//�r�[���̃`���[�W�G�t�F�N�g�Q��
	std::weak_ptr<MyEffect> m_beamChargeEff;
	//�r�[���̃G�t�F�N�g�Q��
	std::weak_ptr<MyEffect> m_beamEff;
	//�r�[���̍U���Q��
	std::weak_ptr<ULT> m_beam;
};

