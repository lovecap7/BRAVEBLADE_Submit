#pragma once
#include "SceneBase.h"
#include <memory>
class SceneController;
class ScreenModeUI;
class VolumeUI;
class Input;
class LeftArrowUI;
class OptionScene :
    public SceneBase
{
public:
    OptionScene(SceneController& controller);
    ~OptionScene();
    //派生クラスで実装を実装
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override;
private:
    //スクリーンモード
    std::weak_ptr<ScreenModeUI> m_screenModeUI;
    //SE
    std::weak_ptr<VolumeUI> m_seUI;
    //BGM
    std::weak_ptr<VolumeUI> m_bgmUI;
    //Voice
    std::weak_ptr<VolumeUI> m_voiceUI;
    //マスター
    std::weak_ptr<VolumeUI> m_masterUI;
	//オプション選択矢印
	std::weak_ptr<LeftArrowUI> m_optionArrowUI;
private:
    enum class OptionIndex : int
    {
        ScreenMode      = 0,
        MasterVolume    = 1,
        BGMVolume       = 2,
        SEVolume        = 3,
        VoiceVolume     = 4,
    };
    OptionIndex m_optionIndex;
    //スクリーンモードの設定
    void OptionSreenMode(Input& input);
    //マスターボリューム
    void OptionMasterVolume(Input& input);
    //BGM
    void OptionBGMVolume(Input& input);
    //SE
    void OptionSEVolume(Input& input);
    //Voice
    void OptionVoiceVolume(Input& input);
};

