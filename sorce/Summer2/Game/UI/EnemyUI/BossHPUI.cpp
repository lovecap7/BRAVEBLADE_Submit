#include "BossHPUI.h"
#include "../UIManager.h"
#include "../../../General/HitPoints.h"
#include "../../Actors/Character/Enemy/EnemyBase.h"
#include "../../../General/game.h"
#include "../../Actors/ActorManager.h"
#include "../../Actors/Stage/BossArea.h"
#include <DxLib.h>

namespace
{
	constexpr float kLeftPosX = 200.0f;
	constexpr float kLeftPosY = Game::kScreenHeight - 100.0f;
	constexpr float kBarHeight = 50.0f;
	constexpr float kBarWidth = Game::kScreenWidth - (kLeftPosX * 2.0f);
	constexpr float kRightPosY = kLeftPosY + kBarHeight;
	//���O�̈ʒu
	constexpr float kNamePosX = kLeftPosX + 10.0f;
	constexpr float kNamePosY = kLeftPosY - 35.0f;
}

BossHPUI::BossHPUI(std::weak_ptr<EnemyBase> enemy) :
	EnemyUIBase(enemy),
	m_viewHp(0.0f),
	m_viewMaxHp(0.0f),
	m_frameHandle(UIManager::GetInstance().GetImageHandle("BossHPFrame")),
	m_hpHandle(UIManager::GetInstance().GetImageHandle("BossHP")),
	m_textHandle(UIManager::GetInstance().GetTextHandle("�����S�V�b�N32")),
	m_name{}
{
	//�G���������ꍇ����UI���폜
	if (m_enemy.expired())
	{
		m_isDelete = true;
		return;
	}
	auto owner = m_enemy.lock();
	auto hp = owner->GetHitPoints().lock();
	//������
	m_viewHp = hp->GetHp();
	m_viewMaxHp = hp->GetMaxHp();
	//���O
	m_name = owner->GetName();
}

BossHPUI::~BossHPUI()
{
}

void BossHPUI::Update()
{
	//�{�X���������ꍇ����UI���폜
	if (m_enemy.expired())
	{
		m_isDelete = true;
		return;
	}
	auto enemy = m_enemy.lock();
	auto hp = enemy->GetHitPoints().lock();
	//�X�V
	//�̗͂ɕϓ�����������
	if (m_viewHp != hp->GetHp())
	{
		m_viewHp = hp->GetHp();
	}
	if (m_viewMaxHp != hp->GetMaxHp())
	{
		m_viewMaxHp = hp->GetMaxHp();
	}
}

void BossHPUI::Draw() const
{
	//�`�悵�Ȃ��Ȃ�return
	if (!m_isDraw)return;
	//�{�X�̖��O
	DrawStringToHandle(kNamePosX, kNamePosY, m_name.c_str(), 0xee0000, m_textHandle);
	DrawBoxAA(kLeftPosX, kLeftPosY, kLeftPosX + (m_viewMaxHp / m_viewMaxHp) * kBarWidth, kRightPosY, 0x555555, true);
	//�̗�
	DrawRectGraph(kLeftPosX, kLeftPosY, 0, 0,
		kBarWidth * (m_viewHp / m_viewMaxHp), kBarHeight, m_hpHandle, true);
	//�t���[��
	DrawGraph(kLeftPosX, kLeftPosY, m_frameHandle, true);
}
