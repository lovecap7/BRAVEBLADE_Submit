#pragma once
#include "PlayerStateBase.h"
#include "../../../../General/Battle.h"
#include "../../../../General/CSVDataLoader.h"
class Actor;
class ULT;
class GameCamera;
class ActorManager;
class MyEffect;
class PlayerStateUltimate :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateUltimate>
{
public:
    PlayerStateUltimate(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager);
    ~PlayerStateUltimate();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //�A�j���[�V�����̃t���[��
    int m_animCountFrame;
    //�A�j���[�V�����̑��x
    float m_animSpeed;
    //�U���̎Q��
    std::weak_ptr<ULT> m_attack;
    //�U���̍쐬
    void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
	//�U���̈ʒu�X�V
    void UpdateLaserPos();
    //�U���f�[�^
    AttackData m_attackData;
    //���̃G�t�F�N�g�Q��
    std::weak_ptr<MyEffect> m_laserChargeEff;
    std::weak_ptr<MyEffect> m_laserEff;
};


