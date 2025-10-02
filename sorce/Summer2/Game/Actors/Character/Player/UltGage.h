#pragma once
class UltGage
{
public:
	UltGage(int max);
	~UltGage();
	//今のゲージ量を返す
	int GetUltGageValue() { return m_ultGageValue; };
	//ゲージを加算
	void AddUltGage(int add);
	//ゲージリセット
	void ResetUltGage() { m_ultGageValue = 0; };
	//必殺技ゲージがマックスならtrue
	bool IsMaxUlt() { return m_ultGageValue >= m_maxUltGageValue; };
	//ゲージの最大値
	int GetMaxUltGageValue() { return m_maxUltGageValue; };
	//加算される予定の値をセット
	void SetPendingUltGage(int pendingUltGage) { m_pendingUltGauge = pendingUltGage; };
	//予定値の反映
	void AddPedingUltGage() { AddUltGage(m_pendingUltGauge); };
private:
	int m_ultGageValue;
	int m_maxUltGageValue;
	//加算される予定の値(攻撃が当たった時に加算される)
	int m_pendingUltGauge;
};

