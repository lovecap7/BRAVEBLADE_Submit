#pragma once
//�t�F�[�h�C���ƃt�F�[�h�A�E�g������N���X
class Fader final
{
private:
	//�V���O���g���̏���
	Fader() = default;
	~Fader() = default;
	//�R�s�[�֎~
	Fader(const Fader&) = delete;
	Fader& operator = (const Fader&) = delete;
	//���[�u�֎~
	Fader(Fader&&) = delete;
	Fader& operator = (Fader&&) = delete;
public:
	/// <summary>
	/// �V���O���g���C���X�^���X���擾����
	/// </summary>
	/// <returns></returns>
	static Fader& GetInstance()
	{
		static Fader instance;
		return instance;
	}
	//������
	void Init();
	//�X�V
	void Update();
	//�`��
	void Draw()const;
	//�t�F�[�h�C��
	void FadeIn(float speed = 5.0f);
	//�t�F�[�h�A�E�g
	void FadeOut(float speed = 5.0f);
	//�t�F�[�h�C�����I��������
	bool IsFinishFadeIn()const;
	//�t�F�[�h�A�E�g���I��������
	bool IsFinishFadeOut()const;
	//�t�F�[�h����
	bool IsFadeNow()const { return m_isFadeNow; };
private:
	//���񂾂񖾂邭�Ȃ�
	bool m_isFadeIn;
	//���񂾂�Â��Ȃ�
	bool m_isFadeOut;
	//�t���[���J�E���g
	float m_countFrame;
	//�t�F�[�h���x
	float m_fadeSpeed;
	//�t�F�[�h��
	bool m_isFadeNow;
};

