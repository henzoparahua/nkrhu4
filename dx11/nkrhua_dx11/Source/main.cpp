#include "../Headers/systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HISTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SytemClass* System;
	bool result;

	System = new SystemClass;

	result = System->Initialize();
	if (result)
	{
		System->Run();
	}
	
	System->Shutdown();
	delete System;
	System = 0;
	
	return 0;
}