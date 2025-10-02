#pragma once
#include <list>
#include <memory>
#include "Attack/AttackBase.h"
#include "Item/ItemBase.h"
#include "../../General/CSVDataLoader.h"
#include "../../General/StageIndex.h"
#include "../Camera/GameCamera/GameCamera.h"
#include <map>

//�L�����N�^�[�̎��
enum class CharacterType : int
{
	Player			= 0,
	PurpleDinosaur	= 1,
	SmallDragon		= 2,
	Bomber			= 3,
	BossDragon		= 4,
	BossMuscle		= 5,
	BossKing		= 6,
};
//�A�C�e���̎��
enum class ItemType : int
{
	Heart			= 0,
	UltGageUp		= 1,
	AttackUp		= 2,
	DefenseUp		= 3,
	Bomb			= 4,
};
//�U���̎��
enum class AttackType : int
{
	Slash			= 0,
	Strike			= 1,
	AreaOfEffect	= 2,
	Bullet			= 3,
	Blast			= 4,
	Breath			= 5,
	ULT				= 6,
	Wave			= 7,
	Homing			= 8,
};

class Player;
class GameCamera;
class Actor;
class BossArea;
class EnemyBase;
class UIManager;
class Score;
class CSVDataLoader;
class GroupManager;
class SearchPlace;
class CharacterBase;
class ActorManager : public std::enable_shared_from_this<ActorManager>
{
public:
	ActorManager(std::weak_ptr<GameCamera> camera);
	~ActorManager();
	//�o�^
	void Entry(std::shared_ptr<Actor> actor);
	//����
	void Exit(std::shared_ptr<Actor> actor);
	//������
	void Init(Stage::StageIndex index);
	//�X�V
	void Update(const std::weak_ptr<Score> score);
	void UpdateDelay();
	//�`��
	void Draw() const;
	//�I������
	void End();
	//���X�^�[�g
	void Restart();

	//�ǉ��\��̃A�N�^�[��ǉ�
	void AddNextActor(std::shared_ptr<Actor> actor);
	//�L�����N�^�[���쐬
	std::weak_ptr<CharacterBase> CreateCharacter(ActorData charaData);
	//�U�����쐬���ĎQ�Ƃ�Ԃ�
	std::weak_ptr<AttackBase> CreateAttack(AttackType at, std::weak_ptr<Actor> owner);
	//�A�C�e���̒ǉ�
	std::weak_ptr<ItemBase> CreateItem(ItemType it, Vector3 pos);
	//�����_���ŃA�C�e���ǉ�
	void CreateRandItem(Vector3 pos);
	//�G���G�����ׂč폜
	void AllDeleteNormalEnemy();
	//�U�������ׂč폜
	void AllDeleteAttack();

	//�v���C���[���擾
	std::weak_ptr<Player> GetPlayer() const { return m_player; };
	//�{�X���擾
	std::weak_ptr<EnemyBase> GetBoss() const { return m_boss; };
	//�{�X����
	std::weak_ptr<BossArea> GetBossArea() const { return m_bossArea; };
	//�v���C���[�ɋ߂��G���擾
	std::weak_ptr<Actor> GetNearestEnemy() const;
	//�U���f�[�^��Ԃ�
	AttackData GetAttackData(std::string& ownerName, std::string& attackName);
	//�{�X�����S������
	bool IsBossDead() const;
	//�{�X�����ł��Đ��t���[���o�߂�����
	bool IsBossDisappear() const;

	//�X�V���~�߂�
	void StopUpdate() { m_isUpdate = false; };
	//�X�V���J�n
	void StartUpdate() { m_isUpdate = true; };
	//�X�V��x�点��
	void DelayUpdate(int frame);
	//�q�b�g�X�g�b�v
	void HitStop(ShakePower sp,int frame);
	//�X�e�[�W�ԍ�
	Stage::StageIndex GetStageIndex()const { return m_stageIndex; };
	//�v���C���[�𕜊�
	void RevivalPlayer();
private:
	//�ǉ��\��̃A�N�^�[������
	void CheckNextAddActors();
	//�폜�\��̃A�N�^�[���폜
	void CheckDeleteActors(const std::weak_ptr<Score> score);
	//�U���f�[�^�̍쐬
	void CreateAttackData();
	//�n���h�����[�h
	void LoadHandle();
	//�X�e�[�W�̏���
	void LoadStage(Stage::StageIndex index);
	//�A�N�^�[�����ׂč폜
	void AllDeleteActors();
	//���ׂăn���h�����폜
	void AllDeleteHandles();
private:
	//�A�N�^�[
	std::list<std::shared_ptr<Actor>> m_actors;
	//�ǉ��\��̃A�N�^�[
	std::list<std::shared_ptr<Actor>> m_nextAddActors;
	//CSV���[�h
	std::unique_ptr<CSVDataLoader> m_csvLoader;
	//�O���[�v�}�l�[�W���[
	std::shared_ptr<GroupManager> m_groupManager;
	//�v���C���[
	std::weak_ptr<Player> m_player;
	//�{�X
	std::weak_ptr<EnemyBase> m_boss;
	//�{�X����
	std::weak_ptr<BossArea> m_bossArea;
	//ID
	int m_actorId;//����U��ԍ�
	//�U���f�[�^
	std::vector<AttackData> m_attackDatas;
	//�J�����̎Q��
	std::weak_ptr<GameCamera> m_camera;
	//�n���h��
	std::map<std::string, int> m_handles;
	//�{�X�����ł��Ă���̃t���[����
	int m_bossDisappearFrame;
	//�X�V
	bool m_isUpdate;
	int m_delayFrame;
	//�X�e�[�W�ԍ�
	Stage::StageIndex m_stageIndex;
};

