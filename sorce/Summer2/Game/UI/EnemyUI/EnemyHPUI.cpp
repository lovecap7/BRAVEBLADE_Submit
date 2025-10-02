#include "EnemyHPUI.h"
#include "../UIManager.h"
#include "../../../General/HitPoints.h"
#include "../../Actors/Character/Enemy/EnemyBase.h"
#include <DxLib.h>

namespace
{
	//�`��ʒu�̒���
	constexpr float kShiftLeftPosX = 70.0f;
	constexpr float kBarHeight = 20.0f;
	constexpr float kBarWidth = kShiftLeftPosX * 2.0f;
	constexpr float kBarOffsetPosY = 150.0f; //�G�̓��̏�ɗ���悤�ɒ���
	//���O�̈ʒu
	constexpr float kNameOffsetPosY = kBarHeight + 20.0f;
	constexpr float kNameOffsetPosX = 10.0f;
}


EnemyHPUI::EnemyHPUI(std::weak_ptr<EnemyBase> enemy) :
	EnemyUIBase(enemy),
	m_viewHp(0.0f),
	m_viewMaxHp(0.0f),
	m_pos{},
	m_frameHandle(UIManager::GetInstance().GetImageHandle("EnemyHPFrame")),
	m_hpHandle(UIManager::GetInstance().GetImageHandle("EnemyHP")),
	m_textHandle(UIManager::GetInstance().GetTextHandle("�����S�V�b�N16")),
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
}

EnemyHPUI::~EnemyHPUI()
{
}

void EnemyHPUI::Update()
{
	//�G���������ꍇ����UI���폜
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
	//���O
	if (m_name != enemy->GetName())
	{
		m_name = enemy->GetName();
	};

	//�`����W���X�V
	m_pos = enemy->GetPos();
	//���̏�ɗ��邮�炢�ɒ���
	m_pos.y += kBarOffsetPosY;
}

void EnemyHPUI::Draw() const
{
	//�`�悵�Ȃ��Ȃ�return
	if (!m_isDraw)return;
	auto pos = ConvWorldPosToScreenPos(m_pos.ToDxLibVector());
	pos.x -= kShiftLeftPosX;
	DrawBoxAA(pos.x, pos.y, pos.x + (m_viewMaxHp / m_viewMaxHp) * kBarWidth, pos.y - kBarHeight, 0x555555, true);
	//�̗�
	DrawRectGraph(pos.x, pos.y - kBarHeight, 0, 0,
		kBarWidth * (m_viewHp / m_viewMaxHp), kBarHeight, m_hpHandle, true);
	//�t���[��
	DrawGraph(pos.x, pos.y - kBarHeight, m_frameHandle, true);
	//���O
	DrawStringToHandle(pos.x + kNameOffsetPosX, pos.y - kNameOffsetPosY, m_name.c_str(), 0xffffff, m_textHandle);
}
