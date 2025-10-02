#include "EffekseerManager.h"
#include "MyEffect.h"
#include "TrackActorEffect.h"
#include <EffekseerForDXLib.h>

namespace
{
	constexpr int kMaxParticleNum = 8000;
}

void EffekseerManager::Entry(std::shared_ptr<MyEffect> eff)
{
	//���łɓo�^����Ă���Ȃ炵�Ȃ�
	auto it = std::find(m_effects.begin(), m_effects.end(), eff);
	if (it != m_effects.end())return;
	//�o�^
	m_effects.emplace_back(eff);
}

void EffekseerManager::Exit(std::shared_ptr<MyEffect> eff)
{
	//�o�^����Ă��Ȃ��Ȃ炵�Ȃ�
	auto it = std::find(m_effects.begin(), m_effects.end(), eff);
	if (it == m_effects.end())return;
	//����
	m_effects.remove(eff);
}
void EffekseerManager::Init()
{
	//�p�[�e�B�N������ݒ�
	Effekseer_Init(kMaxParticleNum);
	//�c�݃G�t�F�N�g�̏�����
	Effekseer_InitDistortion();
	//�t���X�N�؂�ւ����ɂ��������Ȃ�Ȃ��悤�ɐݒ�
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	//�n���h�����[�h
	LoadHandle();
	//������
	m_isUpdate = true;
	m_delayFrame = 0;
}

void EffekseerManager::Update()
{
	//�x������
	UpdateDelay();
	//�X�V�����Ȃ��Ȃ�
	if (!m_isUpdate || m_delayFrame > 0)
	{
		//�Đ��X�g�b�v
		StopEffect();
		return;
	}
	else
	{
		//�Đ�
		StartEffect();
	}
	//�X�V
	for (auto& eff : m_effects)
	{
		eff->Update();
	}
	//�폜�\��̂��̂��폜
	CheckDeleteEffect();
	//�S�ẴG�t�F�N�g�̍X�V
	UpdateEffekseer3D();
}

void EffekseerManager::Draw() const
{
	//Effekseer��3D�\���̐ݒ��DX���C�u������3D�\���̐ݒ�ɓ���������
	Effekseer_Sync3DSetting();
	//�S�ẴG�t�F�N�g�̕`��
	DrawEffekseer3D();
}

void EffekseerManager::End()
{
	//�n���h���폜
	AllDeleteHandle();
	//�G�t�F�N�g�폜
	Reset();
	//�I��
	Effkseer_End();
}

void EffekseerManager::Reset()
{
	//�폜
	for (auto& eff : m_effects)
	{
		eff->End();
	}
	m_effects.clear();
	
}

std::weak_ptr<MyEffect> EffekseerManager::CreateEffect(std::string name, Vector3 pos)
{
	std::shared_ptr<MyEffect> effect;
	effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles[name]), pos);
	Entry(effect);
	return effect;
}

std::weak_ptr<TrackActorEffect> EffekseerManager::CreateTrackActorEffect(std::string name, std::weak_ptr<Actor> actor)
{
	std::shared_ptr<TrackActorEffect> effect;
	effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles[name]), actor);
	Entry(effect);
	return effect;
}

void EffekseerManager::StopEffect()
{
	//�~�߂�
	m_isUpdate = false;
}

void EffekseerManager::StartEffect()
{
	//�Đ�
	m_isUpdate = true;
}

void EffekseerManager::DelayUpdate(int frame)
{
	m_delayFrame = frame;
	m_isUpdate = false;
}

void EffekseerManager::LoadHandle()
{
	m_handles["GetAttackUpEff"] = { LoadEffekseerEffect("Data/Effects/GetAttackUpEff.efkefc") };
	m_handles["AttackUpEff"] = { LoadEffekseerEffect("Data/Effects/AttackUpEff.efkefc") };
	m_handles["GetHealEff"] = { LoadEffekseerEffect("Data/Effects/GetHealEff.efkefc") };
	m_handles["GetDefenseUpEff"] = { LoadEffekseerEffect("Data/Effects/GetDefenseUpEff.efkefc") };
	m_handles["DefenseUpEff"] = { LoadEffekseerEffect("Data/Effects/DefenseUpEff.efkefc") };
	m_handles["GetUltGageUpEff"] = { LoadEffekseerEffect("Data/Effects/GetUltGageUpEff.efkefc") };
	m_handles["SlashtTraject1Eff"] = { LoadEffekseerEffect("Data/Effects/SlashtTraject1Eff.efkefc") };
	m_handles["SlashtTraject2Eff"] = { LoadEffekseerEffect("Data/Effects/SlashtTraject2Eff.efkefc") };
	m_handles["CATornade"] = { LoadEffekseerEffect("Data/Effects/CATornade.efkefc") };
	m_handles["ChargeEff"] = { LoadEffekseerEffect("Data/Effects/ChargeEff.efkefc") };
	m_handles["ChargeLevel1Eff"] = { LoadEffekseerEffect("Data/Effects/ChargeLevel1Eff.efkefc") };
	m_handles["ChargeLevel2Eff"] = { LoadEffekseerEffect("Data/Effects/ChargeLevel2Eff.efkefc") };
	m_handles["ChargeLevel3Eff"] = { LoadEffekseerEffect("Data/Effects/ChargeLevel3Eff.efkefc") };
	m_handles["SlashHitEff"] = { LoadEffekseerEffect("Data/Effects/SlashHitEffect.efkefc") };
	m_handles["ImpactHitEff"] = { LoadEffekseerEffect("Data/Effects/ImpactHitEff.efkefc") };
	m_handles["ExplosionEff"] = { LoadEffekseerEffect("Data/Effects/ExplosionEff.efkefc") };
	m_handles["UltChargeSwordEff"] = { LoadEffekseerEffect("Data/Effects/UltChargeSwordEff.efkefc") };
	m_handles["UltLaserEff"] = { LoadEffekseerEffect("Data/Effects/UltLaserEff.efkefc") };
	m_handles["UltGageMaxEff"] = { LoadEffekseerEffect("Data/Effects/UltGageMaxEff.efkefc") };
	m_handles["UltHitEff"] = { LoadEffekseerEffect("Data/Effects/UltHitEff.efkefc") };
	m_handles["BulletEff"] = { LoadEffekseerEffect("Data/Effects/BulletEff.efkefc") };
	m_handles["BreathEff"] = { LoadEffekseerEffect("Data/Effects/BreathEff.efkefc") };
	m_handles["DeathEff"] = { LoadEffekseerEffect("Data/Effects/DeathEff.efkefc") };
	m_handles["BossDeathEff"] = { LoadEffekseerEffect("Data/Effects/BossDeathEff.efkefc") };
	m_handles["RollingEff"] = { LoadEffekseerEffect("Data/Effects/RollingEff.efkefc") };
	m_handles["LowShockWaveEff"] = { LoadEffekseerEffect("Data/Effects/LowShockWaveEff.efkefc") };
	m_handles["FieldEff"] = { LoadEffekseerEffect("Data/Effects/FieldEffect.efkefc") };
	m_handles["BossBeamEff"] = { LoadEffekseerEffect("Data/Effects/BossBeamEff.efkefc") };
	m_handles["BossBeamChargeEff"] = { LoadEffekseerEffect("Data/Effects/BossBeamChargeEff.efkefc") };
	m_handles["BossKingBeamEff"] = { LoadEffekseerEffect("Data/Effects/BossKingBeamEff.efkefc") };
	m_handles["BossKingBeamChargeEff"] = { LoadEffekseerEffect("Data/Effects/BossKingBeamChargeEff.efkefc") };
	m_handles["BossAngryEff"] = { LoadEffekseerEffect("Data/Effects/BossAngryEff.efkefc") };
	m_handles["WaveAttackEff"] = { LoadEffekseerEffect("Data/Effects/WaveAttackEff.efkefc") };
	m_handles["MagicWaveEff"] = { LoadEffekseerEffect("Data/Effects/MagicWaveEff.efkefc") };
	m_handles["BossKingStartEff"] = { LoadEffekseerEffect("Data/Effects/BossKingStartEff.efkefc") };
	m_handles["BossKingTransEff"] = { LoadEffekseerEffect("Data/Effects/BossKingTransEff.efkefc") };
	m_handles["BlastMagicEff"] = { LoadEffekseerEffect("Data/Effects/BlastMagicEff.efkefc") };
	m_handles["MagicCircleEff"] = { LoadEffekseerEffect("Data/Effects/MagicCircleEff.efkefc") };
	m_handles["BigMagicCircleEff"] = { LoadEffekseerEffect("Data/Effects/BigMagicCircleEff.efkefc") };
	m_handles["RevivalPlayerEff"] = { LoadEffekseerEffect("Data/Effects/RevivalPlayerEff.efkefc") };
	m_handles["AreaWallEff"] = { LoadEffekseerEffect("Data/Effects/AreaWallEff.efkefc") };
	//���[�h�ɐ����������`�F�b�N
	for (const auto& [key, value] : m_handles) {
		assert(value >= 0);
	}
}

void EffekseerManager::AllDeleteHandle()
{
	//�n���h�������ׂč폜
	for (const auto& [key, value] : m_handles) {
		DeleteEffekseerEffect(value);
	}
	m_handles.clear();
}


//�G�t�F�N�g�̏��Ńt���O���`�F�b�N���č폜
void EffekseerManager::CheckDeleteEffect()
{
	std::list<std::shared_ptr<MyEffect>> deleteEffect;
	for (int i = 0;i < 3;++i)
	{
		bool isOneMore = false;
		for (auto& eff : m_effects)
		{
			if (eff->IsDelete())
			{
				isOneMore = true;
				//�I������
				eff->End();
				//�폜���
				deleteEffect.emplace_back(eff);
			}
		}
		//�폜
		for (auto& eff : deleteEffect)
		{
			Exit(eff);
		}
		deleteEffect.clear();
		if (!isOneMore)break;
	}
}
void EffekseerManager::UpdateDelay()
{
	if (m_delayFrame > 0)
	{
		--m_delayFrame;
		if (m_delayFrame <= 0)
		{
			m_isUpdate = true;
		}
	}
}