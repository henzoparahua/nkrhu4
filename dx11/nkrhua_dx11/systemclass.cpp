#include "systemclass.h"

SystemClass::SystemClass()
{
	m_Input = 0;
	m_Application = 0;
}

SystemClass::SystemClass(const SystemClass& other)
{

}

SystemClass::~SystemClass()
{

}


bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	screenHeight = 0;
	screenWidth = 0;

	InitializeWindows(screenWidth, screenHeight);

	m_Input = new InputClass;
	m_Input->Initialize();

	m_Application = new ApplicationClass;

	result = m_Application->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{
	if (m_Application)
	{
		m_Application->Shutdown();
		delete m_Application;
		m_Application = 0;
	}

	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}

	return;

}

bool SystemClass::Frame()
{
	bool result;

	if (m_Input->IsKeyDown(VK_SPACE))
	{
		return false;
	}

	result = m_Application->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}