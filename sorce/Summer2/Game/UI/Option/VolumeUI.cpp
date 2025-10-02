#include "VolumeUI.h"
#include "../../../General/game.h"
#include "../UIManager.h"
#include <DxLib.h>
VolumeUI::VolumeUI(float posY, std::wstring name):
	m_pos{ Game::kScreenCenterX ,posY},
	m_volume(0),
	m_bottunHandle(UIManager::GetInstance().GetImageHandle("SoundBottun")),
	m_textHandle(UIManager::GetInstance().GetTextHandle("ÉÅÉCÉäÉI32")),
	m_name(name)
{
}

VolumeUI::~VolumeUI()
{
}

void VolumeUI::Update()
{
	m_pos.x = Game::kScreenCenterX + (m_volume * 2 - 255);
}

void VolumeUI::Draw() const
{
	DrawBox(Game::kScreenCenterX - 255, m_pos.y - 20, Game::kScreenCenterX + 255, m_pos.y + 20, 0x333333, true);
	DrawRotaGraph(m_pos.x, m_pos.y, 1.0, 0.0, m_bottunHandle, true);
	//è„Ç†ÇΩÇËÇ…çÄñ⁄ñº
	DrawStringToHandle(400, m_pos.y - 60, m_name.c_str(), 0xffffff,m_textHandle);
	//âπó 
	std::wstring volumeText = L"Volume : " + std::to_wstring(m_volume);
	DrawStringToHandle(600, m_pos.y - 60, volumeText.c_str(), 0xffffff, m_textHandle);
}
