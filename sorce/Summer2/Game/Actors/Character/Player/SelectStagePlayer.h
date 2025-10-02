#pragma once
#include "../CharacterBase.h"
#include "../../../../General/Math/MyMath.h"
#include <memory>
class Model;
class Collidable;
class ActorManager;
class Score;
class SelectStagePlayer :
	public CharacterBase
{
public:
	SelectStagePlayer(Vector3 cameraPos, Vector3 pos);
	~SelectStagePlayer();
	//����������
	void Init() override;
	//�X�V����
	void Update(Vector3 cameraPos,Vector3 targetPos);
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override {};
	//�Փ˃C�x���g�֐�
	void OnCollide(const std::shared_ptr<Collidable> other)override {};
	//�`��
	void Draw()const override;
	//�X�V�����ɂ�錋�ʂ̊m��
	void Complete() override {};
	//���S����
	void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override {};
	//�I������
	void End()override;
private:
	//�_���X�`�F���W
	bool m_isChangeDance;
	//����
	bool m_isDecided;
	//�T�E���h
	void InitSound()override;
};
