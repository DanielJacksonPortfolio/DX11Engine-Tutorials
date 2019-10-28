#pragma once
#include <comdef.h>
#include "StringToolkit.h"

#define COM_ERROR_IF_FAILED(hr, msg) if (FAILED(hr)) throw COMException(hr, msg, __FILE__, __FUNCTION__, __LINE__);

class COMException
{
public:
	COMException(HRESULT hr, const std::string& msg, const std::string& file, const std::string& function, int line)
	{
		_com_error error(hr);
		whatmsg = L"Msg: " + StringToolkit::StringToWide(std::string(msg)) + L"\n";
		whatmsg += error.ErrorMessage();
		whatmsg += L"\nFile: " + StringToolkit::StringToWide(std::string(file));
		whatmsg += L"\nFunction: " + StringToolkit::StringToWide(std::string(function));
		whatmsg += L"\nLine: " + StringToolkit::StringToWide(std::to_string(line));
	}

	const wchar_t* what() const
	{
		return whatmsg.c_str();
	}
private:
	std::wstring whatmsg;
};