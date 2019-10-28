#include "ErrorLogger.h"
#include <comdef.h>

void ErrorLogger::Log(std::string msg)
{
	std::string errorMsg = "Error: " + msg;
	MessageBoxA(NULL, errorMsg.c_str(), "Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, std::string msg)
{
	_com_error error(hr);
	std::wstring errorMsg = L"Error: " + StringToolkit::StringToWide(msg)+ L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, errorMsg.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, std::wstring msg)
{
	_com_error error(hr);
	std::wstring errorMsg = L"Error: " + msg + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, errorMsg.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLogger::Log(COMException& exception)
{
	LPCWSTR errorMsg = exception.what();
	MessageBoxW(NULL, errorMsg, L"Error", MB_ICONERROR);
}
