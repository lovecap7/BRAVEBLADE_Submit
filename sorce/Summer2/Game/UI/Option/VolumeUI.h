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
    //ボタンのハンドル
    int m_bottunHandle;
    //テキストハンドル
	int m_textHandle;
	//テキスト
	std::wstring m_name;
};

