#include "TutorialDirecter.h"
#include "Actors/ActorManager.h"
#include "../General/CSVDataLoader.h"
#include "UI/UIManager.h"
#include "Actors/Character/Player/Player.h"
#include "UI/TutorialUI.h"

TutorialDirecter::TutorialDirecter()
{
	//�`���[�g���A���\���ʒu
	auto csvLoder = std::make_shared<CSVDataLoader>();
	auto data = csvLoder->LoadActorDataCSV("Data/CSV/TutorialTransformData.csv");
	for (auto posData : data)
	{
		m_tutorialPosX.emplace_back(posData.pos.x);
	}
	//�\���������Ƃ��t���O�ŊǗ�
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
	//�v���C���[�����݂��Ă��邩�̃`�F�b�N
	if (actorManager.lock()->GetPlayer().expired())return;
	auto player = actorManager.lock()->GetPlayer().lock();

	//�ړ��`���[�g���A���\��
	if (player->GetPos().x > m_tutorialPosX[0] && !m_tutorialViewFlag[TutorialItem::Move])
	{
		ActiveTutorial(TutorialItem::Move, L"[���X�e�B�b�N]��[�ړ�]���ł����!\n[���X�e�B�b�N]���E�ɓ|���ĉE��[�ړ�]���Ă݂悤!!");
	}
	//�ʏ�U���`���[�g���A���\��
	else if (player->GetPos().x > m_tutorialPosX[1] && !m_tutorialViewFlag[TutorialItem::NA])
	{
		ActiveTutorial(TutorialItem::NA, L"[X]��[�ʏ�U��]!!\n[X]�������ēG��|����!!");
	}
	//�W�����v�`���[�g���A���\��
	else if (player->GetPos().x > m_tutorialPosX[2] && !m_tutorialViewFlag[TutorialItem::Jump])
	{
		ActiveTutorial(TutorialItem::Jump, L"[B]��[�W�����v]�I\n���ɗ����Ȃ��悤�ɔ�ڂ�!!");
	}
	//�`���[�W�U���`���[�g���A���\��
	else if (player->GetPos().x > m_tutorialPosX[3] && !m_tutorialViewFlag[TutorialItem::CA])
	{
		ActiveTutorial(TutorialItem::CA, L"[Y]��[�`���[�W�U��]���ł���I\n[Y]�𒷉����Ń`���[�W���ĕ��āI�I");
	}
	//����`���[�g���A���\��
	else if (player->GetPos().x > m_tutorialPosX[4] && !m_tutorialViewFlag[TutorialItem::Rolling])
	{
		ActiveTutorial(TutorialItem::Rolling, L"[A]��[���]���ł���I\n�G�̍U��������Ĕ������I�I�I");
	}
	//�K�E�Z�`���[�g���A���\��
	else if (player->GetPos().x > m_tutorialPosX[5] && !m_tutorialViewFlag[TutorialItem::ULT])
	{
		ActiveTutorial(TutorialItem::ULT, L"HP�o�[�̉��ɂ���K�E�Q�[�W���ő�̎���[RB]��[�K�E�Z]��ł��Ƃ��ł���I\n�悭�_���ċ��͂Ȉꌂ����āI�I�I");
	}

}
void TutorialDirecter::End()
{
	m_tutorialPosX.clear();
	m_tutorialViewFlag.clear();
	//UI�̕\��������
	if (!m_tutorialUI.expired())
	{
		m_tutorialUI.lock()->End();
	}
}

void TutorialDirecter::ActiveTutorial(TutorialItem item, std::wstring text)
{
	//�t���O��true��
	m_tutorialViewFlag[item] = true;
	//UI��\��
	auto ui = std::make_shared<TutorialUI>(text);
	ui->Init();
	//UI�̕\��������
	if (!m_tutorialUI.expired())
	{
		m_tutorialUI.lock()->End();
	}
	m_tutorialUI = ui;
}
