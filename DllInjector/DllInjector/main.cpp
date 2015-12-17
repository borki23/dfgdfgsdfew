#include <cstdio>
#include <string>
#include <iostream>
#include <windows.h> 
#include <TlHelp32.h>

const char* DLL_NAME = "D:/Projects/Multihack/DllInjector/Compiled/multihack.dll";
const char* PROCESS_NAME = "hl.exe";

DWORD readConsoleInst(char* buffer, unsigned int bytesToRead){
	HANDLE stdInput = GetStdHandle(STD_INPUT_HANDLE);

	DWORD saveMode;
	DWORD num;
	GetConsoleMode(stdInput, &saveMode);
	SetConsoleMode(stdInput, ENABLE_PROCESSED_INPUT);

	ReadConsole(stdInput, buffer, bytesToRead, &num, NULL);
	
	SetConsoleMode(stdInput, saveMode);
	return num;
}

HANDLE getProcessHandle(const std::string &name) {
	PROCESSENTRY32 processEntries;
	HANDLE processSnapshot;
	processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32First(processSnapshot, &processEntries)) {
		if (processEntries.szExeFile == name)
		{
			printf("Process: '%'s found\n", name.c_str());
			CloseHandle(processSnapshot);
			HANDLE result = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processEntries.th32ProcessID);
			return result;
		}
		while (Process32Next(processSnapshot, &processEntries))
		{
			if (processEntries.szExeFile == name)
			{
				printf("Process: '%s' found\n", name.c_str());
				CloseHandle(processSnapshot);
				HANDLE result = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processEntries.th32ProcessID);
				return result;
			}
		}
	} else {
		printf("Cannot find any process...");
	}
	CloseHandle(processSnapshot);

	return NULL;
}

int main() {

	HANDLE thisProcess = GetCurrentProcess(), hToken;
	TOKEN_PRIVILEGES privileges;
	LUID luid;

	OpenProcessToken(thisProcess, TOKEN_ADJUST_PRIVILEGES, &hToken);
	LookupPrivilegeValue(0, "seDebugPrivilege", &luid);

	privileges.PrivilegeCount = 1;
	privileges.Privileges[0].Luid = luid;
	privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, false, &privileges, 0, 0, 0);
	printf("%i", GetLastError());
	CloseHandle(hToken);
	CloseHandle(thisProcess);

	char command = 0x00;
	printf("Injector....searching process\nPress 'q' to quit.\n");
	HANDLE gameProcess = NULL;

	while (command != 'q') {
		//printf("Injector....searching process\nPress 'q' to quit.\n");
		//Read console
		//readConsoleInst(&command, 1);
		//command = getchar();
		Sleep(100);
		gameProcess = getProcessHandle(PROCESS_NAME);
		if (gameProcess == NULL) {
			printf("1 ...%i", GetLastError());
		}
 else 
 {
	 break;
 }
	}

	FARPROC libraryFunction = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	if (libraryFunction == NULL) {
		printf("Cannot find 'LoadLibraryA'");
	}

	LPVOID baseAddress = VirtualAllocEx(gameProcess, NULL, std::strlen(DLL_NAME) , MEM_RESERVE|MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (baseAddress == NULL) {
		printf("Cannot allocate memory");
	}

	SIZE_T bytesWritten;
	BOOL memoryWritten = WriteProcessMemory(gameProcess, baseAddress, DLL_NAME, std::strlen(DLL_NAME), &bytesWritten);
	if (!memoryWritten) {
		printf("Could not write memory...");
	}

	HANDLE thread = CreateRemoteThread(gameProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)libraryFunction, baseAddress, NULL, NULL);
	if (thread == NULL) {
		printf("Could not create thread...%i",GetLastError());
	}

	//WaitForSingleObject(thread, INFINITE);

	CloseHandle(gameProcess);

	int c;
	std::cin>> c ;
	return 0;
}