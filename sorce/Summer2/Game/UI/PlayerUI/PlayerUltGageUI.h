#pragma once
#include "PlayerUIBase.h"
#include <memory>
class Player;
class PlayerUltGageUI :
	public PlayerUIBase
{
public:
	PlayerUltGageUI(std::weak_ptr<Player> player);
	~PlayerUltGageUI();
	void Update()override;
	void Draw()const override;
private:
	//ゲージ
	float m_viewUltGageValue;
	float m_viewMaxUltGageValue;	
	//ゲージのフレームハンドル
	int m_ultGageHandle;
	int m_maxUltGageHandle;
	//ゲージの画像ハンドル
	int m_ultGageFrameHandle;
	//ゲージが満タンの時のボタンの画像ハンドル
	int m_ultBottunHandle;
	//ボタンの拡大縮小
	double m_bottunScale;
	//拡大縮小に使う角度
	float m_bottunAngle;
};
