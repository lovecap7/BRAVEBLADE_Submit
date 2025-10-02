#pragma once
#include "../CharacterBase.h"
#include "../../../../General/Math/MyMath.h"
#include <memory>
#include <map>
#include <string>

class ActorManager;
class UltGage;
class GameCamera;
class CharacterStateBase;
class HitPoints;
class TrackActorEffect;
class Input;
class Player :
	public CharacterBase
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	//����������
	void Init()override;
	//�X�V����
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//�Փ˃C�x���g�֐�
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//�`��
	void Draw()const override;
	//�X�V�����ɂ�錋�ʂ̊m��
	void Complete() override;
	//���S����
	void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override;
	//�I������
	void End()override;
	//����
	void Revival();

	//�X�e�[�g�ɃA�N�Z�X������֐�
	//���W�b�h�{�f�B
	std::shared_ptr<Rigidbody> GetRb() const { return m_rb; }
	//�R���W����
	std::shared_ptr<ColliderBase> GetColl() const { return m_collisionData; }
	//�R���W�����̏�Ԃ�ݒ�
	void SetCollState(CollisionState collState) { m_collState = collState; }
	CollisionState GetCollState()const { return m_collState; };

	//���͒��̕����L�[(�v���C���[�̃��f���̌����ɑΉ������x�N�g��)
	Vector2 GetPlayerStickVec() { return m_stickVec; };
	//�K�E�Z�Q�[�W
	std::weak_ptr<UltGage> GetUltGage() const{ return m_ultGage; };
	//�^�[�Q�b�g�̃f�[�^
	TargetData GetTargetData() const { return m_targetData; };
	//�^�[�Q�b�g�̍��G
	virtual void TargetSearch(float searchDistance, float searchAngle, Vector3 targetPos) override;
	//�_�b�V���L�[�v�t���O
	bool IsRunKeep() { return m_isRunKeep; };
	void SetIsRunKeep(bool isRunKeep) { m_isRunKeep = isRunKeep; };

	//���S
	bool IsDead()const { return m_isDead; };
	void SetIsDead(bool isDead) { m_isDead = isDead; };
	//�Q�[���̐i�s�Ɏg���֐�
	//�Q�[���J�n�A�j���[�V���������Ă���
	bool IsStartAnim();
	//�Q�[���N���A�A�j���[�V����
	bool IsClearAnim();
	//�Q�[���N���A�A�j���[�V�����I��
	bool IsFinishClearAnim();
private:
	//�X�e�B�b�N�̌��������x�N�g��
	Vector2 m_stickVec;
	//�K�E�Z�Q�[�W
	std::shared_ptr<UltGage> m_ultGage;
	//�_�b�V����Ԃ��L�[�v����t���O
	bool m_isRunKeep;
	//�_�b�V����Ԃ���������t���[��
	int m_cancelRunFrame;
	//�K�E�Q�[�WMAX�G�t�F�N�g
	std::weak_ptr<TrackActorEffect> m_ultMaxEff;
	//���������Ƃ��ɖ߂����W(�`�F�b�N�|�C���g������������g��)
	Vector3 m_initPos;
	//���񂾃t���O
	bool m_isDead;
private:
	//�v���C���[�̓��̓x�N�g�����X�V
	void UpdatePlayerStickVec(Input& input);
	//������p�����邩
	void CheckRunKeep();
	//�K�E�Q�[�W���ő傩�m�F
	void CheckUltMax();
	//�T�E���h������
	void InitSound() override;
};

