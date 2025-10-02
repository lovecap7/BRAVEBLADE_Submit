#pragma once
#include "../CharacterBase.h"
#include <memory>
#include <string>
#include "../../../../General/Model.h"
class Player;
class HitPoints;
//�G�̊K��
enum class EnemyGrade
{
	Normal,   //�G���G
	Elite,    //���{�X
	Boss      //�{�X
};
class EnemyBase abstract :
    public CharacterBase
{
public:
    EnemyBase(Shape shape, EnemyGrade grade);
    virtual ~EnemyBase() {};
	//�X�V
	virtual void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//�`��
	void Draw() const override;
	//�U���̃N�[���^�C�����X�V
	virtual void UpdateAttackCoolTime();
	//�U���̃N�[���^�C�����擾
	int GetAttackCoolTime() const { return m_attackCoolTime; }
	//�U���̃N�[���^�C�����Z�b�g
	void SetAttackCoolTime(int coolTime) { m_attackCoolTime = coolTime; }
	//�G�̊K��
	EnemyGrade GetEnemyGrade()const { return m_enemyGrade; };
	//�s���\��
	bool IsActive()const { return m_isActive; };
	void SetActive(bool isActive);
	//�x��
	bool IsWarning()const { return m_isWarning; };
	void SetIsWarning(bool isWarning) { m_isWarning = isWarning; };
	//���O
	std::wstring GetName()const { return m_name; };
protected:
	//�U���ł���܂ł̃N�[���^�C��
	int m_attackCoolTime;
	//�����̊K��
	EnemyGrade m_enemyGrade;
	//�s���J�n
	bool m_isActive;
	//�v���C���[�Ƃ̋����������Ƃ��ɓ����蔻����~����
	bool IsStopActiveCollision(const std::weak_ptr<ActorManager> actorManager);
	//�x�����Ă���
	bool m_isWarning;
	//���O
	std::wstring m_name;
};

