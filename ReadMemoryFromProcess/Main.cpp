#include <stdio.h>
#include <iostream>
#include <string.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <sstream>
using namespace std;

DWORD ProcId = 0;
HANDLE GetProcessByName(PCSTR name);
void GetProcId(char* ProcName);

int main(int argc, char *argv[]) {
	HANDLE processHandle;
	string dataToRead;

	char* address = argv[1];
	char* targetProcess = argv[2];

	unsigned int targetAddress;

	if (address == NULL || address == "") {
		printf("Ckya blyat I expected an address :(\n\n");
		system("pause");
		return 0;
	}

	if (targetProcess == NULL || targetProcess == "") {
		printf("Cheeke breeke I expected a process to search into :(\n\n");
		system("pause");
		return 0;
	}

	std::stringstream ss;
	ss << std::hex << address;
	ss >> targetAddress;

	dataToRead.resize(20);

	ZeroMemory(&dataToRead, sizeof(dataToRead));

	GetProcId(targetProcess);

	if (ProcId == 0) {
		printf("Could not find process. Exiting...");
		system("pause");
		return 0;
	}

	processHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, ProcId);

	LPCVOID addressPtr = (LPCVOID)targetAddress;

	BOOL result = ReadProcessMemory(processHandle, addressPtr, &dataToRead, sizeof(dataToRead), 0);

	if (result == 0) {
		DWORD lastError = GetLastError();
		printf("Failed to read process data: " + lastError);
		system("pause");
		return 1;
	}

	cout << "Your data :D" << endl << endl << dataToRead.c_str() << endl << endl;

	ZeroMemory(&dataToRead, sizeof(dataToRead));

	system("pause");

	return 0;
}

void GetProcId(char* ProcName)
{
	PROCESSENTRY32   pe32;
	HANDLE         hSnapshot = NULL;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32First(hSnapshot, &pe32))
	{
		do {
			if (strcmp(pe32.szExeFile, ProcName) == 0)
				break;
		} while (Process32Next(hSnapshot, &pe32));
	}

	if (hSnapshot != INVALID_HANDLE_VALUE)
		CloseHandle(hSnapshot);

	ProcId = pe32.th32ProcessID;
}