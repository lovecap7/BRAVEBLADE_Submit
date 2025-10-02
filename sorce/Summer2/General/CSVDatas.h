#pragma once
#include <string>
#include "Battle.h"
#include "../Game/Camera/GameCamera/GameCamera.h"
#include "Math/MyMath.h"
#include "StageIndex.h"
namespace
{
    //�A�N�^�[���̃X�R�A
    const std::string kActorScoreDataPath = "Data/CSV/ActorScoreData.csv";
    //�n�C�X�R�A
    const std::string kHighScoreDataPath = "Data/CSV/HighScoreData.csv";
    //�U���f�[�^
    const std::string kCharacterAttackDataPath = "Data/CSV/CharacterAttackData.csv";
    //���U���g�X�R�AUI
    const std::string kResutlScoreDataPath = "Data/CSV/ResutlScoreData.csv";
    //�Z�[�u�f�[�^
    const std::string kSaveDataPath = "Data/CSV/SaveData/SaveData.csv";
    //����
    const std::string kVolumeDataPath = "Data/CSV/VolumeData.csv";
}


//�I�u�W�F�N�g�̏����i�[����\����
struct ActorData {
    std::string     name;               //�I�u�W�F�N�g��
    VECTOR          pos;                //�ʒu
    VECTOR          rot;                //��]�i���W�A���j
    VECTOR          scale;              //�X�P�[��
    std::string     gropeTag;           //�O���[�v�^�O
    int             hp;                 //�̗�
    Battle::Armor   armor;              //�A�[�}�[
    int             attackPower;        //�U����
    float           speed;              //���x
    float           searchPlaceRang;    //�T���͈�
    float           viewingAngle;       //����p
    float           searchDistance;     //���G����
    float           modelRotateSpeed;   //���f�����񑬓x
};
//�I�u�W�F�N�g�̏����i�[����\����
struct AttackData {
    std::string                 ownerName;     //�L�����N�^�[��
    std::string                 attackName;	   //�U����
    int                         damege;		   //�_���[�W
    int                         keepFrame;	   //����
    int                         startFrame;	   //����
    float                       knockBackPower;//�m�b�N�o�b�N�̑傫��
    std::string                 anim;	       //�A�j���[�V����
    float                       animSpeed;	   //�A�j���[�V�����̑��x
    float                       radius;        //�U���̔��a
    int                         addUltGage;    //�U�����q�b�g�����Ƃ��̉��Z�Q�[�W��
    int                         moveFrame;     //�O�i����t���[��
    float                       moveSpeed;     //�ړ���
    Battle::AttackWeight        attackWeight;  //�U���̏d��(�������ɋ��ނ��ǂ������v�Z)
    int                         hitStopFrame;  //�q�b�g�X�g�b�v�̒���
    ShakePower                  shakePower;    //�J�����̗h��
    int                         attackNum;     //�U����
};
//�X�R�A�̏����i�[����\����
struct ScoreData {
    std::string dataName;  //�f�[�^��
    int         score;     //�X�R�A
};
//���U���g�X�R�AUI�����i�[����\����
struct ResultScoreUIData {
    std::string     name;   //�X�R�A��
    Vector2         pos;    //�ʒu
    float           scale;  //�傫��
    std::wstring    text;   //��
};
//UI
struct UIData
{
    std::wstring text;  //�e�L�X�g
    Vector2      pos;   //���W
};

struct SaveDatas
{
    //�N���A�X�e�[�W
    bool stage1Clear;
    bool stage2Clear;
    bool stage3Clear;
    //�v���C����
    unsigned int totalPlayTime;
    //���t�Ǝ���
    std::wstring nowTime;
};
//����
struct VolumeData
{
    int masterVolume;
    int bgmVolume;
    int seVolume;
    int voiceVolume;
};