#include <Windows.h>
#include <stdio.h>

DWORD WINAPI dllStart(LPVOID lpParam)
{
	 Beep(100,50);

	//FILE *test;
	//fopen_s(&test, "D:/Projects/Multihack/DllInjector/Debug/log.log", "a+");
	//fprintf(test, "hi");
	//fclose(test);
	return S_OK;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD  fdwReason, LPVOID lpvReserved) {
	HANDLE thread;
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		thread = CreateThread(NULL,0, dllStart, 0, 0, NULL);
		//WaitForSingleObject(thread, INFINITE);
	}
	
	return TRUE;
}

