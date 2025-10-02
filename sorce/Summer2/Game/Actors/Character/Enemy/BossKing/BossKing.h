#pragma once
#include "../EnemyBase.h"
#include <map>
#include <string>
class ActorManager;
class Input;
class GameCamera;
class EnemyManager;
class AttackManager;
class ActorManager;
class CharacterStateBase;
class BossKing :
	public EnemyBase
{
public:
	BossKing(int modelHandle, int transHandle, Vector3 pos);
	~BossKing();
	//����������
	void Init()override;
	//�Փ˃C�x���g
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//�X�V�����̊m��
	void Complete() override;
	//���S����
	void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override;
	//�I������
	void End()override;
	//���W�b�h�{�f�B
	std::shared_ptr<Rigidbody> GetRb() const { return m_rb; }
	//�R���W����
	std::shared_ptr<ColliderBase> GetColl() const { return m_collisionData; }
	//�R���W�����̏�Ԃ�ݒ�
	void SetCollState(CollisionState collState) { m_collState = collState; }
	//�^�[�Q�b�g�̃f�[�^
	TargetData GetTargetData() const { return m_targetData; };
	//�ǂɓ���������
	bool IsWall() const { return m_isWall; }
	//�Q�[���̐i�s�Ɏg���֐�
	//�Q�[���J�n�A�j���[�V���������Ă���
	bool IsStartAnim();
	//�ϐg
	void TransformSecond();
	//�S��
	//�̗͑S��
	void FullRecovery();
private:
	//�ϐg���f��
	int m_transHandle;
	//�T�E���h������
	void InitSound() override;
};

