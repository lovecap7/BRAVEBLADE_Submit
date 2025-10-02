#include "TutorialDirecter.h"
#include "Actors/ActorManager.h"
#include "../General/CSVDataLoader.h"
#include "UI/UIManager.h"
#include "Actors/Character/Player/Player.h"
#include "UI/TutorialUI.h"

TutorialDirecter::TutorialDirecter()
{
	//チュートリアル表示位置
	auto csvLoder = std::make_shared<CSVDataLoader>();
	auto data = csvLoder->LoadActorDataCSV("Data/CSV/TutorialTransformData.csv");
	for (auto posData : data)
	{
		m_tutorialPosX.emplace_back(posData.pos.x);
	}
	//表示したことをフラグで管理
	m_tutorialViewFlag[TutorialItem::Move]		= { false };
	m_tutorialViewFlag[TutorialItem::NA]		= { false };
	m_tutorialViewFlag[TutorialItem::Jump]		= { false };
	m_tutorialViewFlag[TutorialItem::CA]		= { false };
	m_tutorialViewFlag[TutorialItem::Rolling]	= { false };
	m_tutorialViewFlag[TutorialItem::ULT]		= { false };
}

TutorialDirecter::~TutorialDirecter()
{
}

void TutorialDirecter::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//プレイヤーが存在しているかのチェック
	if (actorManager.lock()->GetPlayer().expired())return;
	auto player = actorManager.lock()->GetPlayer().lock();

	//移動チュートリアル表示
	if (player->GetPos().x > m_tutorialPosX[0] && !m_tutorialViewFlag[TutorialItem::Move])
	{
		ActiveTutorial(TutorialItem::Move, L"[左スティック]で[移動]ができるよ!\n[左スティック]を右に倒して右に[移動]してみよう!!");
	}
	//通常攻撃チュートリアル表示
	else if (player->GetPos().x > m_tutorialPosX[1] && !m_tutorialViewFlag[TutorialItem::NA])
	{
		ActiveTutorial(TutorialItem::NA, L"[X]で[通常攻撃]!!\n[X]を押して敵を倒そう!!");
	}
	//ジャンプチュートリアル表示
	else if (player->GetPos().x > m_tutorialPosX[2] && !m_tutorialViewFlag[TutorialItem::Jump])
	{
		ActiveTutorial(TutorialItem::Jump, L"[B]で[ジャンプ]！\n穴に落ちないように飛ぼう!!");
	}
	//チャージ攻撃チュートリアル表示
	else if (player->GetPos().x > m_tutorialPosX[3] && !m_tutorialViewFlag[TutorialItem::CA])
	{
		ActiveTutorial(TutorialItem::CA, L"[Y]で[チャージ攻撃]ができる！\n[Y]を長押しでチャージして放て！！");
	}
	//回避チュートリアル表示
	else if (player->GetPos().x > m_tutorialPosX[4] && !m_tutorialViewFlag[TutorialItem::Rolling])
	{
		ActiveTutorial(TutorialItem::Rolling, L"[A]で[回避]ができる！\n敵の攻撃を避けて反撃だ！！！");
	}
	//必殺技チュートリアル表示
	else if (player->GetPos().x > m_tutorialPosX[5] && !m_tutorialViewFlag[TutorialItem::ULT])
	{
		ActiveTutorial(TutorialItem::ULT, L"HPバーの下にある必殺ゲージが最大の時に[RB]で[必殺技]を打つことができる！\nよく狙って強力な一撃を放て！！！");
	}

}
void TutorialDirecter::End()
{
	m_tutorialPosX.clear();
	m_tutorialViewFlag.clear();
	//UIの表示を消す
	if (!m_tutorialUI.expired())
	{
		m_tutorialUI.lock()->End();
	}
}

void TutorialDirecter::ActiveTutorial(TutorialItem item, std::wstring text)
{
	//フラグをtrueに
	m_tutorialViewFlag[item] = true;
	//UIを表示
	auto ui = std::make_shared<TutorialUI>(text);
	ui->Init();
	//UIの表示を消す
	if (!m_tutorialUI.expired())
	{
		m_tutorialUI.lock()->End();
	}
	m_tutorialUI = ui;
}
