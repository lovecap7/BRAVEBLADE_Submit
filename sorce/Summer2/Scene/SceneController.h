#pragma once
//�X�}�[�g�|�C���^���g��
#include<memory>
#include<list>
class SceneBase;
/// <summary>
/// �e�V�[�����Ǘ�����N���X
/// �Ǘ��͂��邪�A�@�ۂ��Ǘ��͂��Ȃ�(�@�ۂ̓N���X�ǂ������)
/// �C���^�[�t�F�C�X��񋟂���N���X
/// </summary>
class SceneController
{
public:
	SceneController();
	/// <summary>
	/// Application����Ă΂��֐��B���݂̃V�[����Update�����̂܂܎��s�B
	/// </summary>
	
	void Update();
	/// <summary>
	/// Application����Ă΂��֐��B���݂̃V�[����Draw�����̂܂܎��s�B
	/// </summary>
	/// <param name="input"></param>
	void Draw();
	/// <summary>
	/// ���̏�Ԃ��Z�b�g����(�Z�b�g����̂͊e��Ԃ̖���)
	/// �e�V�[�����R���g���[���[�����Đ؂�ւ�������
	/// </summary>
	/// <param name="">���̏�Ԃ̃|�C���^</param>
	void ChangeScene(std::shared_ptr<SceneBase> scene);
	/// <summary>
	/// �ŏ��ɓ��ꂽ�V�[����؂�ւ���
	/// </summary>
	/// <param name=""></param>
	void ChangeBaseScene(std::shared_ptr<SceneBase> scene);
	/// <summary>
	/// �V�[�����v�b�V��(�����)����
	/// </summary>
	/// <param name=""></param>
	void PushScene(std::shared_ptr<SceneBase> scene);
	/// <summary>
	/// �V�[�����|�b�v(���o��)����
	/// </summary>
	/// <param name=""></param>
	void PopScene();
	/// <summary>
	/// �ŏ��ɓ��ꂽ�V�[����������x�����̏�Ԃɖ߂�
	/// </summary>
	void RestartBaseScene();
	/// <summary>
	/// �ŏ��ɓ��ꂽ�V�[���̍X�V
	/// </summary>
	void UpdateBaseScene();
	/// <summary>
	/// �X�V����
	/// </summary>
	void StartUpdate() { m_isUpdate = true; };
	/// <summary>
	/// �X�V���~�߂�
	/// </summary>
	void StopUpdate() { m_isUpdate = false; };
	//���̃V�[���̎Q��
	std::weak_ptr<SceneBase> GetBaseScene() { return m_scenes.front(); };
private:
	using SceneStack_t = std::list<std::shared_ptr<SceneBase>>;//�V�[�����X�^�b�N�\���ɂ��邽�߂̌^
	SceneStack_t m_scenes;//���ݎ��s���̃V�[��(���������͒m��Ȃ�)
	//�V�[���̍X�V
	bool m_isUpdate;
};

