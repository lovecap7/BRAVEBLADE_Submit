#pragma once
#include "../UIBase.h"
#include "../../../General/Math/MyMath.h"
#include <string>
class VolumeUI :
    public UIBase
{
public:
    VolumeUI(float posY, std::wstring name);
    ~VolumeUI();
    void Update()override;
    void Draw() const override;
    void SetVolume(int volume) { m_volume = volume; };
private:
    int m_volume;
    Vector2 m_pos;
    //�{�^���̃n���h��
    int m_bottunHandle;
    //�e�L�X�g�n���h��
	int m_textHandle;
	//�e�L�X�g
	std::wstring m_name;
};

