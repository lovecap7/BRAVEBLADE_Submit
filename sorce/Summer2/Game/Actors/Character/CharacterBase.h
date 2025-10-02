#pragma once
#include "../Actor.h"
#include "../../../General/CSVDatas.h"
#include <memory>
#include <map>
#include <string>
class CharacterStateBase;
class SE;
class Voice;
class CharacterBase abstract:
    public Actor
{
public:
	CharacterBase(Shape shape);
	virtual ~CharacterBase() {};
	//�T�E���h
	std::weak_ptr<SE>  CharacterOnceSE(std::string name);
	std::weak_ptr<SE>  CharacterLoopSE(std::string name);
	std::weak_ptr<Voice>  CharacterVC(std::string name);
	//�L�����N�^�[�̃f�[�^
	void SetCharaData(ActorData data);
protected:
    //�L�����N�^�[�̏��
    std::shared_ptr<CharacterStateBase> m_state;
	//�T�E���h
	std::map<std::string, int> m_soundHandles;
	//�T�E���h������
	virtual void InitSound() abstract;
	//�T�E���h�I������
	virtual void EndSound();
	//����p
	float m_viewingAngle;
	//���G����
	float m_searchDistance;
};

