#pragma once
#include <memory>
#include <string>
#include "../../General/Math/MyMath.h"
#include "../../General/Collision/Collidable.h"

class ActorManager;
class GameCamera;
class Collidable;
class Score;
class HitPoints;
class AttackPoints;
class SearchPlace;
class Model;
/// <summary>
/// �Q�[�����ɔz�u�\�ȕ��̂̊��N���X
/// </summary>
/// //new�ł��Ȃ��Ȃ�
class Actor abstract :
	public Collidable
{
public:
	//�^�[�Q�b�g�Ɋւ���f�[�^�\����
	struct TargetData
	{
		//�^�[�Q�b�g�𔭌�
		bool isHitTarget = false;
		//�^�[�Q�b�g�̍��W
		Position3 targetPos = Vector3::Zero();
		//�^�[�Q�b�g�ւ̌���
		Vector3 targetDirXZ = Vector3::Zero();
		//�^�[�Q�b�g�Ƃ̋���
		float targetDis = 0.0f;
	};
protected:
	//����
	bool m_isDelete;
	//�A�N�^�[�̎��ʔԍ�
	int m_id;
	//ID���Z�b�g���ꂽ���Ƃ��L�^����t���O
	bool m_isSetId;
	//���f��
	std::shared_ptr<Model> m_model;
	//�O���[�v
	std::string m_groupTag;
	//�U����
	bool m_canAttack;
	//�O���[�v�ɏ������Ă��邩
	bool m_isInGroup;
	//�^�[�Q�b�g�̏��
	TargetData m_targetData;
	//�̗�
	std::shared_ptr<HitPoints> m_hitPoints;
	//�U���̃X�e�[�^�X
	std::shared_ptr<AttackPoints> m_attackPoints;
	//�T���ꏊ
	std::shared_ptr<SearchPlace> m_searchPlace;
public:
	Actor(Shape shape);
	virtual ~Actor() {};
	/// <summary>
	/// ����������
	/// </summary>
	virtual void Init()abstract;
	/// <summary>
	/// �ʒu�Ȃǂ̍X�V���s��(�����Ȃ�)
	/// </summary>
	virtual void Update() {};
	/// <summary>
	/// �ʒu�Ȃǂ̍X�V���s��
	/// </summary>
	virtual void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) abstract;
	/// <summary>
	/// �Ώۂ̃A�N�^�[��\��
	/// </summary>
	virtual void Draw()const abstract;
	/// <summary>
	/// ���S�����ۂ̏���
	/// </summary>
	/// <param name="actorManager"></param>
	virtual void Dead(const std::weak_ptr<ActorManager> actorManager,const std::weak_ptr<Score> score)abstract;
	/// <summary>
	/// �I������
	/// </summary>
	virtual void End()abstract;
	/// <summary>
	/// �폜�t���O
	/// </summary>
	/// <returns></returns>
	bool IsDelete() { return m_isDelete; };
	/// <summary>
	/// �폜�\��
	/// </summary>
	/// <returns></returns>
	void Delete() { m_isDelete = true; };
	/// <summary>
	/// ID���擾
	/// </summary>
	/// <returns></returns>
	int GetID() const { return m_id; };
	/// <summary>
	/// ID���Z�b�g
	/// </summary>
	/// <returns></returns>
	void SetID(int id);
	/// <summary>
	/// ���W
	/// </summary>
	Vector3 GetPos() const;
	/// <summary>
	/// ���̍��W
	/// </summary>
	Vector3 GetNextPos() const;
	//���f���N���X
	std::shared_ptr<Model> GetModel() const { return m_model; };
	//�O���[�v
	std::string GetGroupTag()const { return m_groupTag; };
	void SetGroupTag(std::string& tag);
	//�O���[�v�ɏ������Ă��邩
	bool IsInGroup()const { return m_isInGroup; };
	//�U����
	bool CanAttack()const { return m_canAttack; };
	void SetCanAttack(bool canAttack) { m_canAttack = canAttack; };
	//�^�[�Q�b�g�̍��G
	virtual void TargetSearch(float searchDistance, float searchAngle, Vector3 targetPos);
	//�^�[�Q�b�g������
	void LookAtTarget();
	//�^�[�Q�b�g�𔭌��ł�����
	bool IsSearch()const { return m_targetData.isHitTarget; };
	//�̗͂̃X�e�[�^�X
	std::weak_ptr<HitPoints> GetHitPoints() const { return m_hitPoints; };
	//�U���̃X�e�[�^�X
	std::weak_ptr<AttackPoints> GetAttackPoints() const { return m_attackPoints; };
	//�U�����󂯂��Ƃ��̏���
	virtual void OnHitFromAttack(const std::shared_ptr<Collidable> other);
	//�T���ꏊ�̎Q��
	std::weak_ptr<SearchPlace> GetSearchPlace() { return m_searchPlace; };
	//�T���ꏊ�����邩
	bool IsHaveSearchPlace()const;
	//�T���͈�
	void SetSearchPlaceRang(float rang);
};

