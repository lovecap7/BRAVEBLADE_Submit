#pragma once
#include <memory>
#include <vector>
#include <map>
#include <string>

enum class TutorialItem
{
	Move,
	NA,
	Jump,
	CA,
	Rolling,
	ULT,
};

class ActorManager;
class TutorialUI;
/// <summary>
/// �`���[�g���A���̐i�s������N���X
/// </summary>
class TutorialDirecter
{
public:
	TutorialDirecter();
	~TutorialDirecter();
	void Update(const std::weak_ptr<ActorManager> actorManager);
	void End();
private:
	//�`���[�g���A���\���ʒu
	std::vector<int> m_tutorialPosX;
	//�`���[�g���A����\��������
	std::map<TutorialItem, bool> m_tutorialViewFlag;
	//UI
	std::weak_ptr<TutorialUI> m_tutorialUI;
	//�\������`���[�g���A��
	void ActiveTutorial(TutorialItem item, std::wstring text);
};

