#include <Windows.h>
#include <stdio.h>

DWORD WINAPI dllStart(LPVOID lpParam)
{
	Beep(100, 200);

	//FILE *test;
	//fopen_s(&test, "D:/Projects/Multihack/DllInjector/Debug/log.log", "a+");
	//fprintf(test, "hi");
	//fclose(test);
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD  fdwReason, LPVOID lpvReserved) {
	CreateThread(NULL, 0, dllStart, 0,0,NULL);
	return TRUE;
}

