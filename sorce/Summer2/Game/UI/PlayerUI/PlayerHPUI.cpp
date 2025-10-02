#include "PlayerHPUI.h"
#include "../../../General/HitPoints.h"
#include "../../Actors/Character/Player/Player.h"
#include "../../Actors/ActorManager.h"
#include "../UIManager.h"
#include <DxLib.h>

namespace
{
	constexpr float kBarHeight = 50.0f;
	constexpr float kBarWidth = 400.0f;
	constexpr float kLeftPosX = 50.0f;
	constexpr float kLeftPosY = 50.0f;
	constexpr float kRightPosY = kLeftPosY + kBarHeight;
}

PlayerHPUI::PlayerHPUI(std::weak_ptr<Player> player) :
	PlayerUIBase(player),
	m_viewHp(0.0f),
	m_viewMaxHp(0.0f),
	m_hpFrameHandle(UIManager::GetInstance().GetImageHandle("PlayerGageFrame")),
	m_hpHandle(UIManager::GetInstance().GetImageHandle("PlayerHP"))
{
	//�v���C���[���������ꍇ����UI���폜
	if (m_player.expired())
	{
		m_isDelete = true;
		return;
	}
	auto hp = m_player.lock()->GetHitPoints().lock();
	//������
	m_viewHp = hp->GetHp();
	m_viewMaxHp = hp->GetMaxHp();
}

PlayerHPUI::~PlayerHPUI()
{
}

void PlayerHPUI::Update()
{
	if (!m_isDraw)return;
	//�v���C���[���������ꍇ�͂���UI���폜
	if (m_player.expired())
	{
		m_isDelete = true;
		return;
	}
	auto player = m_player.lock();
	//�v���C���[���Q�[���J�n�A�j���[�V���������N���A�A�j���[�V�������Ȃ�`�悵�Ȃ�
	if (player->IsStartAnim() || player->IsClearAnim())
	{
		m_isDraw = false;
	}
	else
	{
		m_isDraw = true;
	}
	auto hp = player->GetHitPoints().lock();
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

void PlayerHPUI::Draw() const
{
	//�`�悵�Ȃ��Ȃ�return
	if (!m_isDraw)return;
	DrawBoxAA(kLeftPosX, kLeftPosY, kLeftPosX + (m_viewMaxHp / m_viewMaxHp) * kBarWidth, kRightPosY, 0x555555, true);
	//�̗�
	DrawRectGraph(kLeftPosX, kLeftPosY, 0 , 0,
		kBarWidth * (m_viewHp / m_viewMaxHp), kBarHeight,m_hpHandle, true);
	//�t���[��
	DrawGraph(kLeftPosX, kLeftPosY, m_hpFrameHandle, true);
}
