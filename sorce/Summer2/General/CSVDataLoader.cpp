#include <fstream>   // �t�@�C���ǂݍ��ݗp
#include <sstream>   // �����񕪉�p�istringstream�jstring���t�@�C���̂悤�Ɉ�����
#include "CSVDataLoader.h"
#include "StringUtil.h"
namespace
{
	//�A�N�^�[�̃f�[�^��
	constexpr int kActorDataElementNum = 19;
	//�U���̃f�[�^��
	constexpr int kAttackDataElementNum = 16;
	//�X�R�A�f�[�^�̐�
	constexpr int kScoreDataElementNum = 2;
	//�X�R�A�f�[�^�̐�
	constexpr int kResultScoreUIDataElementNum = 4;
	//UI�f�[�^��
	constexpr int kUIDataElementNum = 3;
	//�Z�[�u�f�[�^�̗v�f��
	constexpr int kSaveDataElementNum = 5;
	//���ʂ̗v�f��
	constexpr int kVolumeDataElementNum = 4;
	//Unity�̍��W�Ɋ|���邱�Ƃ�DX���C�u�����ł�Unity�Ɠ����傫���ɂȂ�
	constexpr float kUnityToDXPosition = 100.0f;
}

CSVDataLoader::CSVDataLoader()
{
}

CSVDataLoader::~CSVDataLoader()
{
}

std::vector<ActorData> CSVDataLoader::LoadActorDataCSV(const char* fileName)
{
	//�f�[�^���i�[����z��
	std::vector<ActorData> objects;

	//�f�[�^�����ׂēǂݍ���
	auto valuesDatas = GetStringList(fileName, kActorDataElementNum);

	for(auto values: valuesDatas)
	{
		//�\���̂Ƀf�[�^�����Ă���
		ActorData objData;
		
		//���O
		objData.name = values[0];
		//���W
		objData.pos.x	= std::stof(values[1]) * kUnityToDXPosition;	//std::stof�͕������float�ɕϊ�����
		objData.pos.y	= std::stof(values[2]) * kUnityToDXPosition;
		objData.pos.z	= std::stof(values[3]) * kUnityToDXPosition;
		//��]
		objData.rot.x	= std::stof(values[4]);	
		objData.rot.y	= std::stof(values[5]);
		objData.rot.z	= -1 * std::stof(values[6]);
		//�傫��
		objData.scale.x = std::stof(values[7]);	
		objData.scale.y = std::stof(values[8]);
		objData.scale.z = std::stof(values[9]);
		//�O���[�v
		objData.gropeTag = values[10];
		//�̗�
		objData.hp = std::stoi(values[11]);
		//�A�[�}�[
		objData.armor = static_cast<Battle::Armor>(std::stoi(values[12]));
		//�U����
		objData.attackPower = std::stoi(values[13]);
		//���x
		objData.speed = std::stof(values[14]);
		//�T���͈�
		objData.searchPlaceRang = std::stof(values[15]);
		//����p
		objData.viewingAngle = std::stof(values[16]) * MyMath::DEG_2_RAD;
		//���G����
		objData.searchDistance = std::stof(values[17]);
		//���f���̐��񑬓x
		objData.modelRotateSpeed = std::stof(values[18]);

		//�z��ɒǉ�
		objects.emplace_back(objData);
	}

	return objects;
}

std::vector<AttackData> CSVDataLoader::LoadAttackDataCSV()
{
	//�f�[�^���i�[����z��
	std::vector<AttackData> attackDatas;
	//�f�[�^�����ׂēǂݍ���
	auto valuesDatas = GetStringList(kCharacterAttackDataPath.c_str(), kAttackDataElementNum);
	for (auto values : valuesDatas)
	{
		//�\���̂Ƀf�[�^�����Ă���
		AttackData attackData;
		//������̖��O
		attackData.ownerName = values[0];
		//�U���̖��O
		attackData.attackName = values[1];
		//�_���[�W
		attackData.damege = std::stoi(values[2]);
		//����
		attackData.keepFrame = std::stoi(values[3]);
		//����
		attackData.startFrame = std::stoi(values[4]);
		//�m�b�N�o�b�N�̑傫��
		attackData.knockBackPower = std::stof(values[5]);
		//�A�j���[�V����
		attackData.anim =  values[6];
		//�A�j���[�V�������x
		attackData.animSpeed = std::stof(values[7]);
		//�U���̔��a
		attackData.radius = std::stof(values[8]);
		//�K�E�Q�[�W�̏㏸��
		attackData.addUltGage = std::stoi(values[9]);
		//�O�i�t���[��
		attackData.moveFrame = std::stoi(values[10]);
		//�O�i���x
		attackData.moveSpeed = std::stof(values[11]);
		//�U���̏d��
		auto aw = values[12];
		if		(aw == "Light")		attackData.attackWeight	= Battle::AttackWeight::Light;
		else if (aw == "Middle")	attackData.attackWeight	= Battle::AttackWeight::Middle;
		else if (aw == "Heavy")		attackData.attackWeight	= Battle::AttackWeight::Heavy;
		else if (aw == "Heaviest")	attackData.attackWeight = Battle::AttackWeight::Heaviest;
		//�q�b�g�X�g�b�v�̒���
		attackData.hitStopFrame = std::stoi(values[13]);
		//�J�����̗h��
		auto sp = values[14];
		if		(sp == "None")		attackData.shakePower = ShakePower::None;
		else if (sp == "Low")		attackData.shakePower = ShakePower::Low;
		else if (sp == "Middle")	attackData.shakePower = ShakePower::Middle;
		else if (sp == "High")		attackData.shakePower = ShakePower::High;
		else if (sp == "Highest")	attackData.shakePower = ShakePower::Highest;
		//�U����
		attackData.attackNum = std::stoi(values[15]);
		//�z��ɒǉ�
		attackDatas.emplace_back(attackData);
	}
	return attackDatas;
}

std::vector<ScoreData> CSVDataLoader::LoadActorScoreDataCSV()
{
	//�f�[�^���i�[����z��
	std::vector<ScoreData> scoreDatas;
	//�f�[�^�����ׂēǂݍ���
	auto valuesDatas = GetStringList(kActorScoreDataPath.c_str(), kScoreDataElementNum);
	for (auto values : valuesDatas)
	{
		//�\���̂Ƀf�[�^�����Ă���
		ScoreData scoreData;
		//������̖��O
		scoreData.dataName = values[0];
		//�U���̖��O
		scoreData.score = std::stoi(values[1]);
		//�z��ɒǉ�
		scoreDatas.emplace_back(scoreData);
	}
	return scoreDatas;
}

std::array<std::array<int, 3>, static_cast<int>(Stage::StageIndex::StageNum)> CSVDataLoader::LoadHighScoreDataCSV()
{
	//�f�[�^���i�[����z��
	std::array<std::array<int, 3>, static_cast<int>(Stage::StageIndex::StageNum)> highScoreDatas;
	//�f�[�^�����ׂēǂݍ���
	auto valuesDatas = GetStringList(kHighScoreDataPath.c_str(), 3);
	for (int i = 0;i < highScoreDatas.size(); ++i)
	{
		//�z��ɒǉ�(�e�X�e�[�W��1�ʂ���3�ʂ܂ł̃n�C�X�R�A���擾)
		highScoreDatas[i][0] = std::stoi(valuesDatas[i][0]);
		highScoreDatas[i][1] = std::stoi(valuesDatas[i][1]);
		highScoreDatas[i][2] = std::stoi(valuesDatas[i][2]);
	}
	return highScoreDatas;
}

std::vector<ResultScoreUIData> CSVDataLoader::LoadResultScoreUIDataCSV()
{
	//�f�[�^���i�[����z��
	std::vector<ResultScoreUIData> resultScoreUIDatas;
	//�f�[�^�����ׂēǂݍ���
	auto valuesDatas = GetWStringList(kResutlScoreDataPath.c_str(), kResultScoreUIDataElementNum);
	for (auto values : valuesDatas)
	{
		//�\���̂Ƀf�[�^�����Ă���
		ResultScoreUIData resultScoreUIData;
		//���O
		resultScoreUIData.name = StringUtil::WstringToString(values[0]);
		//���W
		resultScoreUIData.pos.x = std::stof(values[1]);
		resultScoreUIData.pos.y = std::stof(values[2]);
		//�e�L�X�g
		resultScoreUIData.text = values[3];
		//�z��ɒǉ�
		resultScoreUIDatas.emplace_back(resultScoreUIData);
	}
	return resultScoreUIDatas;
}

std::vector<UIData> CSVDataLoader::LoadUIDataCSV(const char* fileName)
{
	//�f�[�^���i�[����z��
	std::vector<UIData> uis;

	//�f�[�^�����ׂēǂݍ���
	auto uiDatas = GetWStringList(fileName, kUIDataElementNum);

	for (auto data : uiDatas)
	{
		//�\���̂Ƀf�[�^�����Ă���
		UIData uiData;
		//�e�L�X�g
		uiData.text = data[0];
		//���W
		uiData.pos.x = std::stof(data[1]);
		uiData.pos.y = std::stof(data[2]);
		//�z��ɒǉ�
		uis.emplace_back(uiData);
	}

	return uis;
}

SaveDatas CSVDataLoader::LoadSaveDataCSV()
{
	//�f�[�^��ۑ�����ϐ�
	SaveDatas saveDatas;
	//�f�[�^�����ׂēǂݍ���
	auto valuesDatas = GetWStringList(kSaveDataPath.c_str(), kSaveDataElementNum);
	//�f�[�^���i�[���Ă�
	for (auto data : valuesDatas)
	{		
		//�N���A�����X�e�[�W�̃��[�h
		saveDatas.stage1Clear = (data[0] == L"1");
		saveDatas.stage2Clear = (data[1] == L"1");
		saveDatas.stage3Clear = (data[2] == L"1");
		//���v���C����
		saveDatas.totalPlayTime = std::stoi(data[3]);
		//���t�Ǝ���
		saveDatas.nowTime = data[4];
	}
	return saveDatas;
}

VolumeData CSVDataLoader::LoadVolumeDataCSV()
{
	//�f�[�^��ۑ�����ϐ�
	VolumeData volumeDatas;
	//�f�[�^�����ׂēǂݍ���
	auto valuesDatas = GetStringList(kVolumeDataPath.c_str(), kVolumeDataElementNum);
	//�f�[�^���i�[���Ă�
	for (auto data : valuesDatas)
	{
		//���ʃZ�b�g
		volumeDatas.masterVolume	= stoi(data[0]);
		volumeDatas.bgmVolume		= stoi(data[1]);
		volumeDatas.seVolume		= stoi(data[2]);
		volumeDatas.voiceVolume		= stoi(data[3]);
	}
	return volumeDatas;
}

//�f�[�^�����ׂēǂݍ���
const std::vector<std::vector<std::string>> CSVDataLoader::GetStringList(const char* fileName, int elementNum)
{
	//�Ԃ��l
	std::vector<std::vector<std::string>> valuesDatas;
	//�t�@�C�����J��
	std::ifstream file(fileName);
	//�������t�@�C�����J���Ȃ�������
	if (!file.is_open())return valuesDatas;//��̃��X�g��Ԃ�
	//1�s���ǂݎ��p�̕ϐ�
	std::string line;
	//�ŏ��̃w�b�_�[�̓X�L�b�v������
	bool isHeader = true;
	//CSV�̏I���܂œǂݎ��
	// getline�œǂݎ���Ă���(�ǂݎ��ʒu�i�����́u�|�C���^�v�j�́A���[�v�̂��тɑO�ɐi�݂܂�)
	//1�s���ǂݎ���Ă����ǂݎ��s���Ȃ��Ȃ�����false�ɂȂ�
	while (std::getline(file, line))//1�s���ǂݎ��
	{
		//�ŏ��̍s�̓X�L�b�v����(�w�b�_�[)
		if (isHeader)
		{
			isHeader = false;
			continue;
		}
		//�s���J���}��؂��1���ǂݍ��ނ��߂̏���
		std::stringstream ss(line);			//��������X�g���[��(getline�œǂݎ�邽��)�ɕϊ�
		std::string part;					//�������Ď��o����1�v�f
		std::vector<std::string> values;	//�v�f���܂Ƃ߂��z��
		//�J���}��؂�Ŏ��o���Ă���
		//ss����,��؂�Ŏ��o���Ă���part�ɓ���Ă���
		while (std::getline(ss, part, ',')) {
			values.emplace_back(part);           //�������ꂽ���ڂ����X�g�ɒǉ�
		}
		//�v�f���`�F�b�N
		if (values.size() < elementNum)continue;//�Ȃ��ꍇ�͕s���ȍs�Ȃ̂Ŕ�΂�
		//�f�[�^��z��ɒǉ�
		valuesDatas.emplace_back(values);
	}
	//�Öك��[�u������̂ł����炭���v
	return valuesDatas;
}

const std::vector<std::vector<std::wstring>> CSVDataLoader::GetWStringList(const char* fileName, int elementNum)
{
	//�Ԃ��l
	std::vector<std::vector<std::wstring>> valuesDatas;
	//�t�@�C�����J��
	std::wifstream file(fileName);

	//���P�[����ݒ�i���ˑ��A�K�v�ɉ�����UTF-8�ȂǂɕύX�j
	//���P�[���͕������߂̃��[���̂���
	//����CSV�t�@�C���� UTF-8 �ŕۑ�����Ă���ꍇ�A
	//Windows�� std::wifstream �̓f�t�H���g��UTF-8�𐳂������߂ł��Ȃ��̂Őݒ肵��
	file.imbue(std::locale("")); 

	//�������t�@�C�����J���Ȃ�������
	if (!file.is_open())return valuesDatas;//��̃��X�g��Ԃ�
	//1�s���ǂݎ��p�̕ϐ�
	std::wstring line;
	//�ŏ��̃w�b�_�[�̓X�L�b�v������
	bool isHeader = true;
	//CSV�̏I���܂œǂݎ��
	// getline�œǂݎ���Ă���(�ǂݎ��ʒu�i�����́u�|�C���^�v�j�́A���[�v�̂��тɑO�ɐi�݂܂�)
	//1�s���ǂݎ���Ă����ǂݎ��s���Ȃ��Ȃ�����false�ɂȂ�
	while (std::getline(file, line))//1�s���ǂݎ��
	{
		//�ŏ��̍s�̓X�L�b�v����(�w�b�_�[)
		if (isHeader)
		{
			isHeader = false;
			continue;
		}
		//�s���J���}��؂��1���ǂݍ��ނ��߂̏���
		std::wstringstream ss(line);			//��������X�g���[��(getline�œǂݎ�邽��)�ɕϊ�
		std::wstring part;					//�������Ď��o����1�v�f
		std::vector<std::wstring> values;	//�v�f���܂Ƃ߂��z��
		//�J���}��؂�Ŏ��o���Ă���
		//ss����,��؂�Ŏ��o���Ă���part�ɓ���Ă���
		while (std::getline(ss, part, L',')) {
			values.emplace_back(part);           //�������ꂽ���ڂ����X�g�ɒǉ�
		}
		//�v�f���`�F�b�N
		if (values.size() < elementNum)continue;//�Ȃ��ꍇ�͕s���ȍs�Ȃ̂Ŕ�΂�
		//�f�[�^��z��ɒǉ�
		valuesDatas.emplace_back(values);
	}
	//�Öك��[�u������̂ł����炭���v
	return valuesDatas;
}
