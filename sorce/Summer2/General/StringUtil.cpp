#include "StringUtil.h"
#include <cassert>

TCHAR* StringUtil::ToTCHAR(const char* c)
{
	// char* �� TCHAR* �ɕϊ�
	TCHAR cT[256];
	MultiByteToWideChar(CP_ACP, 0, c, -1, cT, 256);
	return cT;
}

std::wstring StringUtil::InsertNewLines(const std::wstring& str, int maxLength)
{
	std::wstring newStr;
	//���炩���ߕK�v�ȃ��������m��(�������œK��)
	newStr.reserve(str.size() + (str.size() / maxLength));
	//�����𐔂��Ă���
	size_t count = 0;
	for (auto c : str)
	{
		//���������Ă���
		newStr += c;
		//���s�����܂������J�E���g��0��
		if (c == '\n')
		{
			count = 0;
			continue;
		}
		//�J�E���g
		++count;
		//��s�̍ő啶�����𒴂�����
		if (count > maxLength)
		{
			//���s
			newStr += '\n';
			//���Z�b�g
			count = 0;
		}
	}
	return newStr;
}

int StringUtil::WStringLineNum(const std::wstring& str)
{
	size_t count = 0;
	for (char c : str) {
		if (c == '\n') {
			count++;
		}
	}
	return static_cast<int>(count);
}

std::string StringUtil::WstringToString(const std::wstring& wstr)
{
	std::string ret;
	//��x�ڂ̌Ăяo���͕����񐔂�m�邽��
	auto result = WideCharToMultiByte(
		CP_ACP,
		0,
		wstr.c_str(),//���͕�����
		wstr.length(),
		nullptr,
		0,
		nullptr,
		nullptr);
	assert(result >= 0);
	ret.resize(result);//�m�ۂ���
	//��x�ڂ̌Ăяo���͕ϊ�
	result = WideCharToMultiByte(
		CP_ACP,
		0,
		wstr.c_str(),//���͕�����
		wstr.length(),
		ret.data(),
		ret.size(),
		nullptr,
		nullptr);
	return ret;
}

std::wstring StringUtil::StringToWstring(const std::string& str)
{
	std::wstring ret;
	//��x�ڂ̌Ăяo���͕����񐔂�m�邽��
	auto result = MultiByteToWideChar(CP_UTF8,
		0,
		str.c_str(),//���͕�����
		str.length(),
		nullptr,
		0);
	assert(result >= 0);
	ret.resize(result);//�m�ۂ���
	//��x�ڂ̌Ăяo���͕ϊ�
	result = MultiByteToWideChar(CP_UTF8,
		0,
		str.c_str(),//���͕�����
		str.length(),
		ret.data(),
		ret.size());
	return ret;
}