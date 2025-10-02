#pragma once
#include <memory>
#include <list>
#include <map>
#include <string>
#include "../Math/MyMath.h"

class MyEffect;
class TrackActorEffect;
class Actor;
/// <summary>
/// �G�t�F�N�g���Ǘ�����V���O���g���N���X
/// </summary>
class EffekseerManager final
{
private:
	//�V���O���g���̏���
	EffekseerManager() = default;
	~EffekseerManager() = default;
	//�R�s�[�֎~
	EffekseerManager(const EffekseerManager&) = delete;
	EffekseerManager& operator = (const EffekseerManager&) = delete;
	//���[�u�֎~
	EffekseerManager(EffekseerManager&&) = delete;
	EffekseerManager& operator = (EffekseerManager&&) = delete;
public:
	//�C���X�^���X���擾
	static EffekseerManager& GetInstance()
	{
		static EffekseerManager instance;
		return instance;
	}
	//�o�^
	void Entry(std::shared_ptr<MyEffect> eff);
	//����
	void Exit(std::shared_ptr<MyEffect> eff);
	//������
	void Init();
	//�X�V����
	void Update();
	//�`��
	void Draw()const;
	//�I������
	void End();
	//���ׂẴG�t�F�N�g���폜
	void Reset();
	//�G�t�F�N�g���쐬
	std::weak_ptr<MyEffect> CreateEffect(std::string name, Vector3 pos);
	std::weak_ptr<TrackActorEffect> CreateTrackActorEffect(std::string name, std::weak_ptr<Actor> actor);
	//�G�t�F�N�g���~�߂�
	void StopEffect();
	//�G�t�F�N�g���J�n
	void StartEffect();
	//���t���[���~�߂�
	void DelayUpdate(int frame);
private:
	//�n���h��
	std::map<std::string, int> m_handles;
	//�G�t�F�N�g�̔z��
	std::list<std::shared_ptr<MyEffect>> m_effects;
	//�X�V���邩
	bool m_isUpdate;
	//�X�V��x�点��
	int m_delayFrame;
private:
	//�n���h�����[�h
	void LoadHandle();
	//�n���h�������ׂč폜
	void AllDeleteHandle();
	//�폜���̃G�t�F�N�g���폜
	void CheckDeleteEffect();
	//�X�V��x��������t���[���̏���
	void UpdateDelay();
};

