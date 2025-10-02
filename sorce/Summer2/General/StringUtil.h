#pragma once
#include <DxLib.h>
#include <string>

namespace StringUtil
{

	/// <summary>
	/// char -> TCHA
	/// </summary>
	/// <param name="c"></param>
	/// <returns></returns>
	TCHAR* ToTCHAR(const char* c);

	/// <summary>
	/// �����������鐔�𒴂��邽�тɉ��s���͂��݁A���̕������Ԃ�
	/// </summary>
	/// <param name="str"></param>
	/// <param name="maxLength"></param>
	/// <returns></returns>
	std::wstring InsertNewLines(const std::wstring& str, int maxLength);

	/// <summary>
	/// �s�̐�
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	int WStringLineNum(const std::wstring& str);

	/// <summary>
	/// wstring����string��
	/// </summary>
	std::string WstringToString(const std::wstring& wstr);

	/// <summary>
	/// string����wstring��
	/// </summary>
	std::wstring StringToWstring(const std::string& str);
}