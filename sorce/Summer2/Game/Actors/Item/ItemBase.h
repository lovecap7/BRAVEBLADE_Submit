#pragma once
#include "../Actor.h"

class ActorManager;
class ItemBase abstract:
    public Actor
{
public:
    ItemBase(Shape shape);
    ~ItemBase();
	//����������
	void Init()override;
	//�X�V����
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//�`��
	void Draw()const override;
	//�X�V�����̊m��
	void Complete() override;
	//�I������
	void End()override;
protected:
    //�ŏ��̐��t���[���͓����蔻����������Ȃ��̂�
    int m_noHitFrame;
};

