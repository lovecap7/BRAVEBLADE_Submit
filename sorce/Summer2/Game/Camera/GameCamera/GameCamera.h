#pragma once
#include <memory>
#include "../../../General/Math/MyMath.h"
#include "../../../General/StageIndex.h"

enum class ShakePower : int
{
	None = 0,
	Low = 5,
	Middle = 10,
	High = 20,
	Highest = 40,
};

class ActorManager;
class Rigidbody;
class EventAreaBase;
class GameCameraStateBase;
class GameCamera:
	public std::enable_shared_from_this<GameCamera>
{
public:
	GameCamera();
	~GameCamera();
	//����������
	void Init(Stage::StageIndex stageIndex);
	//�X�V����
	void Update(const std::weak_ptr<ActorManager> actorManager);
	//�J�����̍��W
	Position3 GetPos() const { return m_pos; };
	void SetPos(Position3 pos) { m_pos = pos; };
	//�J�����̌���
	Vector3 GetDir()const;
	void SetDir(Vector3 dir);
	//�J�����̌��Ă���ʒu
	Vector3 GetViewPos()const { return m_viewPos; };
	void SetViewPos(Vector3 viewPos) { m_viewPos = viewPos; };
	//�C�x���g�G���A
	std::weak_ptr<EventAreaBase> GetEventArea()const { return m_eventArea; };
	void SetEventArea(std::weak_ptr<EventAreaBase> event) { m_eventArea = event; };
	//�J�����V�F�C�N
	void UpdateCameraShake();
	void SetCameraShake(ShakePower power,int frame);
private:
	//�����̍��W
	Position3 m_pos;
	//�J�����̌���
	Vector3 m_dir;
	//�J���������Ă�ʒu
	Vector3 m_viewPos;
	//�C�x���g�G���A
	std::weak_ptr<EventAreaBase> m_eventArea;
	//�U��
	ShakePower m_shakePower;
	int m_shakeFrame;
	int m_maxShakeFrame;
	//��ԑJ��
	std::shared_ptr<GameCameraStateBase> m_state;
};

