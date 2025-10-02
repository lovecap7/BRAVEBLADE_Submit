#pragma once
#include <vector>
#include <memory>
#include "../General/StageIndex.h"

class GameCamera;
class ActorManager;
class UIManager;
class Timer;
class TutorialDirecter;
class GameManager
{
public:
    GameManager();
    ~GameManager();
    void Init(Stage::StageIndex index = Stage::StageIndex::Stage1);
    void Update();
    void Draw()const;
    void End();
    //���X�^�[�g
    void Restart(Stage::StageIndex index = Stage::StageIndex::Stage1);
    //�Q�[���I�[�o�[
    bool IsGameover() { return m_isGameover; };
    //�Q�[���N���A
    bool IsGameClear() { return m_isGameClear; };
	//���U���g
    bool IsResult() { return m_isResult; };
    //�R���e�j���[
    void Continue();
private:
	//�A�N�^�[�}�l�[�W���[
	std::shared_ptr<ActorManager> m_actorManager;
    //�J����
    std::shared_ptr<GameCamera> m_camera;
    //�^�C�}�[
    std::shared_ptr<Timer> m_timer;
    //�`���[�g���A��
    std::shared_ptr<TutorialDirecter> m_tutorialDirecter;
private:
    //�Q�[���I�[�o�[�t���O
    bool m_isGameover;
    //�Q�[���N���A�t���O
    bool m_isGameClear;
    //���U���g�t���O
	bool m_isResult;
    //�e
    int m_shadowMapHandle;
    //���C�g�n���h��
	std::vector<int> m_lightHandles;
    //�f�o�b�O���[�h�p
    bool m_isUpdateStop = false;
private:
    //���C�g�̏�����
    void InitLight();
    //�V���h�E�}�b�v�̏�����
    void InitShadow();
    //�`��͈͍X�V
    void UpdateShadowDrawArea();
    //�Q�[���J�n���̏���
    void UpdateGameStart();
};
