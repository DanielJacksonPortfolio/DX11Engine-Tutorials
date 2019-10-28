#pragma once
#include "ErrorLogger.h"

class WindowContainer;

class RenderWindow
{
private:
	void RegisterWindowClass();
	HWND handle = NULL;
	HINSTANCE hInstance = NULL;
	std::string windowTitle = "";
	std::wstring windowTitleW = L"";
	std::string windowClass = "";
	std::wstring windowClassW = L"";
	int width, height = 0;
public:
	bool Init(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height);
	bool ProcessMessages();
	~RenderWindow();
	const HWND GetHandle();
};

