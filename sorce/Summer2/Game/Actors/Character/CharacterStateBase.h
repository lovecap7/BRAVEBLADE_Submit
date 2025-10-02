#pragma once
#include <memory>
#include "../../../General/Math/MyMath.h"
#include "../Actor.h"
namespace
{
	//���C�L���X�g�̋��e�l(�����ƃ^�[�Q�b�g)
	constexpr int kRayCastBeforeMoveToleranceNum = 2;
	//�ړ���̃��C�L���X�g�̋��e�l(�^�[�Q�b�g�̂�)
	constexpr int kRayCastAfterMoveToleranceNum = 1;
}

class Input;
class GameCamera;
class ActorManager;
class CharacterStateBase abstract
{
public:
	CharacterStateBase(std::weak_ptr<Actor> owner);
	virtual ~CharacterStateBase();
	//����������
	virtual void Init()abstract;
	//��Ԃɉ������X�V����
	virtual void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) abstract;
	//���̏�Ԃ��擾
	std::shared_ptr<CharacterStateBase> GetNextState() { return m_nextState; };
	//��ԕω�
	virtual void ChangeState(std::shared_ptr<CharacterStateBase> nextState);
	//������̎Q��
	std::weak_ptr<Actor> GetOwner() const { return m_owner; }
private:
	//���̏��
	std::shared_ptr<CharacterStateBase> m_nextState;
protected:
	//�����̃|�C���^
	std::weak_ptr<Actor> m_owner;
	//���̈ړ��x�N�g�������C�L���X�g�̌��ʂ����ƂɎ擾
	Vector3 GetNextNomVecFromRayCast(std::shared_ptr<Actor> coll, Vector3 targetPos, Vector3 moveDir, float speed, 
		int beforeMoveToleranceNum = kRayCastBeforeMoveToleranceNum,int afterMoveToleranceNum = kRayCastAfterMoveToleranceNum);
};

