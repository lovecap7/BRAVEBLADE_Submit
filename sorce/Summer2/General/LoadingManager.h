#pragma once
#include <map>
#include <string>
#include <list>
#include <memory>
/// <summary>
///�񓯊����[�h
/// </summary>
class LoadingManager final
{
private:
	//�V���O���g���̏���
	LoadingManager() = default;
	~LoadingManager() = default;
	//�R�s�[�֎~
	LoadingManager(const LoadingManager&) = delete;
	LoadingManager& operator = (const LoadingManager&) = delete;
	//���[�u�֎~
	LoadingManager(LoadingManager&&) = delete;
	LoadingManager& operator = (LoadingManager&&) = delete;
public:
	//�C���X�^���X���擾
	static LoadingManager& GetInstance()
	{
		static LoadingManager instance;
		return instance;
	}
	//������
	void Init();
	//�X�V
	void Update();
	//�`��
	void Draw() const;
	//�I��
	void End();
	//�񓯊����[�h�J�n
	void StartLoading();
	//�I��
	void StopLoading();
	//�񓯊����[�h��
	bool IsLoading()const;
private:
	//���[�h��
	int m_loadingNum;
	//�񓯊����[�h�t���[�����J�E���g
	int m_countFrame;
};
