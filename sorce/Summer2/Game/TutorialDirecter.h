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
/// チュートリアルの進行をするクラス
/// </summary>
class TutorialDirecter
{
public:
	TutorialDirecter();
	~TutorialDirecter();
	void Update(const std::weak_ptr<ActorManager> actorManager);
	void End();
private:
	//チュートリアル表示位置
	std::vector<int> m_tutorialPosX;
	//チュートリアルを表示したか
	std::map<TutorialItem, bool> m_tutorialViewFlag;
	//UI
	std::weak_ptr<TutorialUI> m_tutorialUI;
	//表示するチュートリアル
	void ActiveTutorial(TutorialItem item, std::wstring text);
};

