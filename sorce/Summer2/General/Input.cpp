#include <Dxlib.h>
#include "Input.h"
#include "Math/Vector2.h"

namespace
{
	//�g���K�[�̉������݋
	constexpr float kTriggerPower = 128;
	//�X�e�B�b�N�̓��͐����̑傫��
	constexpr int kLeftStickPowerX = 20;
	constexpr int kLeftStickPowerY = 20;
	//���������|���Ă���Ɣ��肷��͈�
	constexpr float kLowPowerStickMin = 50.0f;
	constexpr float kLowPowerStickMax = 400.0f;
	//���������|���Ă���Ɣ��肷��͈�
	constexpr float kMediumPowerStickMin = kLowPowerStickMax;
	constexpr float kMediumPowerStickMax = 700.0f;
	//�ő�܂œ|���Ă���Ɣ��肷��͈�
	constexpr float kHighPowerStickMin = kMediumPowerStickMax;
}

void Input::Init()
{
	m_inputActionMap["Ok"]		= { {InputType::kKeyboard,KEY_INPUT_RETURN}	, {InputType::kPad,PAD_INPUT_A} };
	m_inputActionMap["Pause"]	= { {InputType::kKeyboard,KEY_INPUT_P}		, {InputType::kPad,PAD_INPUT_8} };
	m_inputActionMap["Start"]	= { {InputType::kKeyboard,KEY_INPUT_P}		, {InputType::kPad,PAD_INPUT_8} };
	m_inputActionMap["Select"]	= { {InputType::kKeyboard,KEY_INPUT_TAB}	, {InputType::kPad,PAD_INPUT_6} };
	m_inputActionMap["LS"]		= { {InputType::kKeyboard,KEY_INPUT_LSHIFT}	, {InputType::kPad,PAD_INPUT_START} };
	m_inputActionMap["RS"]		= { {InputType::kKeyboard,KEY_INPUT_RSHIFT}	, {InputType::kPad,PAD_INPUT_9} };
	m_inputActionMap["Cancel"]	= { {InputType::kKeyboard,KEY_INPUT_ESCAPE}	, {InputType::kPad,PAD_INPUT_B} };
	m_inputActionMap["Up"]		= { {InputType::kKeyboard,KEY_INPUT_W}		, {InputType::kPad,PAD_INPUT_UP} };
	m_inputActionMap["Down"]	= { {InputType::kKeyboard,KEY_INPUT_S}		, {InputType::kPad,PAD_INPUT_DOWN} };
	m_inputActionMap["Left"]	= { {InputType::kKeyboard,KEY_INPUT_A}		, {InputType::kPad,PAD_INPUT_LEFT} };
	m_inputActionMap["Right"]	= { {InputType::kKeyboard,KEY_INPUT_D}		, {InputType::kPad,PAD_INPUT_RIGHT} };
	m_inputActionMap["LB"]		= { {InputType::kKeyboard,KEY_INPUT_J}		, {InputType::kPad,PAD_INPUT_5} };
	m_inputActionMap["RB"]		= { {InputType::kKeyboard,KEY_INPUT_L}		, {InputType::kPad,PAD_INPUT_6} };
	m_inputActionMap["A"]		= { {InputType::kKeyboard,KEY_INPUT_U}		, {InputType::kPad,PAD_INPUT_1} };
	m_inputActionMap["B"]		= { {InputType::kKeyboard,KEY_INPUT_H}		, {InputType::kPad,PAD_INPUT_2} };
	m_inputActionMap["X"]		=	{ {InputType::kKeyboard,KEY_INPUT_G}	, {InputType::kPad,PAD_INPUT_3} };
	m_inputActionMap["Y"]		= { {InputType::kKeyboard,KEY_INPUT_Y}		, {InputType::kPad,PAD_INPUT_4} };

	//�f�o�b�O�p
	m_inputActionMap["SceneChange"]		= { {InputType::kKeyboard,KEY_INPUT_1} };
	m_inputActionMap["StopUpdate"]		= { {InputType::kKeyboard,KEY_INPUT_2} };
	m_inputActionMap["OneFrame"]		= { {InputType::kKeyboard,KEY_INPUT_I} };
	m_inputActionMap["Enter"]			= { {InputType::kKeyboard,KEY_INPUT_RETURN} };
	m_inputActionMap["Enter"]			= { {InputType::kKeyboard,KEY_INPUT_RETURN} };
	m_inputActionMap["Max"]				= { {InputType::kKeyboard,KEY_INPUT_M} ,{InputType::kPad,PAD_INPUT_5} };
	m_inputActionMap["BossArea"]		= { {InputType::kKeyboard,KEY_INPUT_B}};
	m_inputActionMap["BossDead"]		= { {InputType::kKeyboard,KEY_INPUT_N}};
	m_inputActionMap["Clear"]			= { {InputType::kKeyboard,KEY_INPUT_C}};
	m_inputActionMap["BossAngry"]		= { {InputType::kKeyboard,KEY_INPUT_A}};
	m_inputActionMap["PlayerDead"]		= { {InputType::kKeyboard,KEY_INPUT_D}};
	m_inputActionMap["AllClearStage"]	= { {InputType::kKeyboard,KEY_INPUT_A}};

	//�X�V������
	m_isUpdate = true;
}

void Input::Update()
{
	//�O�̃t���[���̓��͏���ۑ�����
	m_lastInput = m_currentInput;

	if (!m_isUpdate)return;

	//���ׂĂ̓��͂��擾����
	char keyState[256] = {};
	int padState = {};
	int mouseState = {};
	GetHitKeyStateAll(keyState);
	padState = GetJoypadInputState(DX_INPUT_PAD1);
	mouseState = GetMouseInput();

	//�A�N�V�������Ɋ��蓖�Ă��Ă��邷�ׂẴL�[�̓��͂��`�F�b�N����
	for (const auto& keyInfo : m_inputActionMap)
	{
		bool isPress = false;
		for (const auto& inputInfo : keyInfo.second)
		{
			//�L�[�{�[�h�̃`�F�b�N
			if (inputInfo.type == InputType::kKeyboard && keyState[inputInfo.buttonID])
			{
				isPress = true;
			}
			//�p�b�h�̃`�F�b�N
			if (inputInfo.type == InputType::kPad && padState & inputInfo.buttonID)
			{
				isPress = true;
			}
			//�}�E�X�̃`�F�b�N
			if (inputInfo.type == InputType::kMouse && mouseState & inputInfo.buttonID)
			{
				isPress = true;
			}

			if (isPress)
			{
				//���͂�����Ȃ�J�E���g
				m_repeateCountFrame[keyInfo.first]++;
				break;
			}
		}
		//���݂̃t���[���ŉ�����Ă������ǂ�����Ԃ�
		m_currentInput[keyInfo.first] = isPress;
		if (!isPress)
		{
			//������ĂȂ��Ȃ烊�Z�b�g
			m_repeateCountFrame[keyInfo.first] = 0;
		}
	}

	//�X�e�B�b�N�̓��͂�����������
	m_stickInfo.leftStickX = 0;
	m_stickInfo.leftStickY = 0;
	m_stickInfo.rightStickX = 0;
	m_stickInfo.rightStickY = 0;

	//�X�e�B�b�N�̓��͂��擾����
	DxLib::GetJoypadAnalogInput(&m_stickInfo.leftStickX, &m_stickInfo.leftStickY, DX_INPUT_PAD1);
	DxLib::GetJoypadAnalogInputRight(&m_stickInfo.rightStickX, &m_stickInfo.rightStickY, DX_INPUT_PAD1);
	XINPUT_STATE* xInputState = new XINPUT_STATE;
	DxLib::GetJoypadXInputState(DX_INPUT_PAD1, xInputState);
	m_triggerInfo.left = xInputState->LeftTrigger;
	m_triggerInfo.right = xInputState->RightTrigger;
}

void Input::StopUpdate()
{
	for (auto& item : m_currentInput)
	{
		item.second = false;
	}
	for (auto& item : m_lastInput)
	{
		item.second = false;
	}

	m_stickInfo.leftStickX = 0;
	m_stickInfo.leftStickY = 0;
	m_stickInfo.rightStickX = 0;
	m_stickInfo.rightStickY = 0;

	m_triggerInfo.left = 0;
	m_triggerInfo.right = 0;

	//�X�V���Ƃ߂�
	m_isUpdate = false;
}

bool Input::IsPress(const std::string& action)const
{
	auto keyInfo = m_currentInput.find(action);

	//�{�^��������`����Ă��Ȃ�������false��Ԃ�
	if (keyInfo == m_currentInput.end())
	{
		return false;
	}
	else
	{
		//����������bool�̒l��Ԃ�
		return keyInfo->second;
	}
}

bool Input::IsTrigger(const std::string& action)const
{
	if (IsPress(action))
	{
		//�O�̃t���[�����Q��
		auto last = m_lastInput.find(action);
		//����`�̃{�^������������false��Ԃ�
		if (last == m_lastInput.end())
		{
			return false;
		}
		else
		{
			//�O�̃t���[���ł�������Ă�����
			return !last->second;
		}
	}
	else
	{
		return false;
	}
}

bool Input::IsRelease(const std::string& action)const
{
	//�܂�������Ă��邩�𔻒�
	if (IsPress(action))
	{
		//������Ă�����false��Ԃ�
		return false;
	}
	else
	{
		//�O�̃t���[�����Q��
		auto last = m_lastInput.find(action);
		//������Ă�����false��Ԃ�
		if (last == m_lastInput.end())
		{
			return false;
		}
		//������Ă��Ȃ����
		else
		{
			return last->second;
		}
	}

	return false;
}

bool Input::IsLowPowerLeftStick()const
{
	//���������|���Ă���Ȃ�true
	Vector2 stickVec;
	stickVec.x = m_stickInfo.leftStickX;
	stickVec.y = m_stickInfo.leftStickY;
	float length = stickVec.Magnitude();
	if (length >= kLowPowerStickMin && length < kLowPowerStickMax)
	{
		return true;
	}
	return false;
}

bool Input::IsMediumPowerLeftStick()const
{
	//���������|���Ă���Ȃ�true
	Vector2 stickVec;
	stickVec.x = m_stickInfo.leftStickX;
	stickVec.y = m_stickInfo.leftStickY;
	float length = stickVec.Magnitude();
	if (length >= kMediumPowerStickMin && length < kMediumPowerStickMax)
	{
		return true;
	}
	return false;
}

bool Input::IsHighPowerLeftStick() const
{
	//�|���؂��Ă�Ȃ�true
	Vector2 stickVec;
	stickVec.x = m_stickInfo.leftStickX;
	stickVec.y = m_stickInfo.leftStickY;
	float length = stickVec.Magnitude();
	if (length >= kHighPowerStickMin)
	{
		return true;
	}
	return false;
}


bool Input::IsPushTrigger(bool right, int power)
{
	//�g���K�[�̏�Ԏ擾
	auto trigger = GetTriggerInfo();

	//�E���𔻒肷��ꍇ
	if (right)
	{
		//�������������������܂ꂽ��
		if (trigger.right >= power)
		{
			return true;
		}
	}
	//�����𔻒肷��ꍇ
	else
	{
		//�������������������܂ꂽ��
		if (trigger.left >= power)
		{
			return true;
		}
	}
	return false;
}

bool Input::IsPushTrigger(bool right)
{
	return IsPushTrigger(right, kTriggerPower);
}

bool Input::IsPressAny()const
{
	return	IsPress("Ok")		||
			IsPress("Pause")	||
			IsPress("Start")	||
			IsPress("Select")	||
			IsPress("LS")		||
			IsPress("RS")		||
			IsPress("Cancel")	||
			IsPress("LB")		||
			IsPress("RB")		||
			IsPress("A")		||
			IsPress("B")		||
			IsPress("X")		||
			IsPress("Y");
}

bool Input::IsTriggerAny()const
{
	return	IsTrigger("Ok")		||
			IsTrigger("Pause")	||
			IsTrigger("Start")	||
			IsTrigger("Select") ||
			IsTrigger("LS")		||
			IsTrigger("RS")		||
			IsTrigger("Cancel") ||
			IsTrigger("LB")		||
			IsTrigger("RB")		||
			IsTrigger("A")		||
			IsTrigger("B")		||
			IsTrigger("X")		||
			IsTrigger("Y");
}

bool Input::IsRepeate(const std::string& action, int deltaTime)const
{
	//���͂���������
	if (IsPress(action))
	{
		//���Ԋu�ŕԂ�
		if (m_repeateCountFrame.at(action) % deltaTime == 0 || IsTrigger(action))
		{
			return true;
		}
	}
	return false;
}
