#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include "Employee.h"
#include "Command.h"



int main(int argc, char** argv) {
	std::cout << "Enter server ID:";
	int id;
	scanf("%d", &id);
	char WritePipe[100];
	char ReadPipe[100];
	char lb = false;
	sprintf(WritePipe, "\\\\.\\pipe\\Pipe_%d_%d", 1, id);
	std::cout << WritePipe << std::endl;
	if (!WaitNamedPipe(WritePipe, NMPWAIT_USE_DEFAULT_WAIT))
		lb = 1;
	sprintf(ReadPipe, "\\\\.\\pipe\\Pipe_%d_%d", 2, id);
	std::cout << ReadPipe << std::endl;
	if (!WaitNamedPipe(ReadPipe, NMPWAIT_USE_DEFAULT_WAIT))
		lb = 1;
	if (lb) {
		std::cout << "connection failed";
		return 1;
	}

	HANDLE hWriteHandle = CreateFile(WritePipe,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES)NULL,
		OPEN_EXISTING,
		0,
		(HANDLE)NULL);

	HANDLE hReadHandle = CreateFile(ReadPipe,
		GENERIC_READ,
		FILE_SHARE_WRITE,
		(LPSECURITY_ATTRIBUTES)NULL,
		OPEN_EXISTING,
		0,
		(HANDLE)NULL);


	char comm;
	DWORD bytes;
	employee st;
	command cm;
	std::cout << "\tm - modify\n\tr - read\n\tq - quit\n";
	std::cout << "\nEnter command:";
	while (std::cin >> comm) {
		if (comm == 'q') {
			cm.type = 3;
			WriteFile(hWriteHandle, &cm, sizeof(cm), &bytes, 0);
			break;
		}
		else
			if (comm == 'r')
			{
				cm.type = 0;
				std::cout << "Enter employee's number:";
				std::cin >> cm.num;
				WriteFile(hWriteHandle, &cm, sizeof(cm), &bytes, 0);
				ReadFile(hReadHandle, &cm, sizeof(cm), &bytes, 0);
				if (cm.result == true) {
					system("pause");
					ReadFile(hReadHandle, (char*)&st, sizeof(st), &bytes, 0);
					std::cout << "\nEmployee's number number: " << st.num;
					std::cout << "\nName: " << st.name;
					std::cout << "\nHours: " << st.hours << std::endl;
				}
				else
					std::cout << "No such record: " << cm.num;

			}
			else if (comm == 'm') {
				cm.type = 1;
				std::cout << "Enter employee's number number:";
				std::cin >> cm.num;
				employee st;
				WriteFile(hWriteHandle, &cm, sizeof(cm), &bytes, 0);
				ReadFile(hReadHandle, &cm, sizeof(cm), &bytes, 0);
				if (cm.result == true) {
					ReadFile(hReadHandle, (char*)&st, sizeof(st), &bytes, 0);
					std::cout << "Employee's number number: " << st.num;
					std::cout << "\nName: " << st.name;
					std::cout << "\Hours: " << st.hours << std::endl << std::endl;
					std::cout << "\nEnter  new employee's number number: ";
					std::cin >> st.num;
					std::cin.get();
					std::cout << "Enter new name: ";
					std::cin.getline(st.name, 9);
					std::cout << "Enter new hours: ";
					std::cin >> st.hours;
					system("pause");
					WriteFile(hWriteHandle, &st, sizeof(st), &bytes, 0);
				}
				else
					std::cout << "No such record:: " << cm.num;
			}
			else
				std::cout << "No such command!";
		std::cout << "\nEnter command:";
	}
}