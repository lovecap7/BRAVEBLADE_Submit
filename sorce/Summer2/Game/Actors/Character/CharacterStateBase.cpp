#include "CharacterStateBase.h"
#include "../../../General/Collision/Physics.h"

CharacterStateBase::CharacterStateBase(std::weak_ptr<Actor> owner):
	m_owner(owner)
{
}

CharacterStateBase::~CharacterStateBase()
{
}

void CharacterStateBase::ChangeState(std::shared_ptr<CharacterStateBase> nextState)
{
	//��ԕω�
	m_nextState = move(nextState);
}

Vector3 CharacterStateBase::GetNextNomVecFromRayCast(std::shared_ptr<Actor> coll, Vector3 targetPos, Vector3 moveDir, float speed, int beforeMoveToleranceNum, int afterMoveToleranceNum)
{
	//�����̈ʒu
	Vector3 myPos = coll->GetPos();
	//�ړ��x�N�g��
	Vector3 moveVec = moveDir;
	if (moveVec.SqMagnitude() > 0.0f)
	{
		moveVec = moveVec.Normalize();
	}
	//���C���΂�
	auto& physics = Physics::GetInstance();
	//�Ղ镨������Ȃ�(�������鐔��葽���Ȃ�true)
	auto amamam = physics.RayCast(myPos, targetPos).size();
	if (physics.RayCast(myPos, targetPos).size() > beforeMoveToleranceNum)
	{
		// ����x�N�g�����i�E�E���j
		Vector3 right = moveVec.Cross(Vector3::Up());
		if (right.SqMagnitude() > 0.0f)right = right.Normalize();
		Vector3 left = Vector3::Up().Cross(moveVec);
		if (left.SqMagnitude() > 0.0f)left = left.Normalize();

		Vector3 rightPos = myPos + right * speed;
		Vector3 leftPos = myPos + left * speed;

		//�E�����ɉ���ł��邩(�������鐔�ȉ��Ȃ�true)
		if (physics.RayCast(rightPos, targetPos).size() <= afterMoveToleranceNum)
		{
			moveVec = right;
		}
		else
		{
			moveVec = left;
		}
	}
	return moveVec;
}
