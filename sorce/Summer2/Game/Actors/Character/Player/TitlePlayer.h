#pragma once
#include "../CharacterBase.h"
#include "../../../../General/Math/MyMath.h"
#include <memory>
class Model;
class Collidable;
class ActorManager;
class Score;
class TitlePlayer :
	public CharacterBase
{
public:
	TitlePlayer();
	~TitlePlayer();
	//����������
	void Init() override;
	//�X�V����
	void Update(bool isDecide);
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
	void InitSound()override;
};
