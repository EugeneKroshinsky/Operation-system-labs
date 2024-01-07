#define _CRT_SECURE_NO_WARNINGS

#include "Employee.h"
#include "Command.h"
#include <iostream>
#include <fstream>
#include <windows.h>


#define MAXFILENAMELEN 20

std::fstream file;
char* filename;
CRITICAL_SECTION cs;
HANDLE* hWriteAccess;
HANDLE* hReadAccess;
int* countOfReaders;

void init() {
	std::cout << "Enter filename: ";
	std::cin.getline(filename, MAXFILENAMELEN - 1);
	file.open(filename, std::ios::binary | std::ios::out | std::ios::in | std::ios::trunc);
	std::cout << "Enter record count: ";
	int N;
	std::cin >> N;
	hWriteAccess = new HANDLE[N];
	hReadAccess = new HANDLE[N];
	countOfReaders = new int[N];
	file.write((char*)&N, sizeof(int));
	for (int i = 0; i < N; ++i) {
		countOfReaders[i] = 0;
		employee st;
		hWriteAccess[i] = CreateEvent(0, TRUE, TRUE, NULL);
		hReadAccess[i] = CreateEvent(0, TRUE, TRUE, NULL);
		std::cout << "\t\nRecord number " << i + 1 << ":\n";
		std::cout << "Enter employee's number: ";
		std::cin >> st.num;
		std::cout << "Enter name: ";
		std::cin.get();
		std::cin.getline(st.name, 9);
		std::cout << "Enter hours: ";
		std::cin >> st.hours;
		file.write((char*)&st, sizeof(st));
	}
}

struct PipeInfo {
	HANDLE hReadHandle;
	HANDLE hWriteHandle;
	int		myNum;
};

bool lookUp(int num, employee& st, DWORD& pos) {
	EnterCriticalSection(&cs);
	file.seekg(0, std::ios::beg);
	int N;
	file.read((char*)&N, sizeof(int));
	for (pos = 0; pos < N; ++pos) {
		file.read((char*)&st, sizeof(st));
		if (st.num == num)
			break;
	}
	LeaveCriticalSection(&cs);
	if (pos == N)
		return false;
	return true;
}

void write(employee st, DWORD pos) {
	EnterCriticalSection(&cs);
	file.seekp(sizeof(int) + pos * sizeof(st), std::ios::beg);
	file.write((char*)&st, sizeof(st));
	LeaveCriticalSection(&cs);
}

void read(employee& st, DWORD pos) {
	EnterCriticalSection(&cs);
	file.seekp(sizeof(int) + pos * sizeof(st), std::ios::beg);
	file.read((char*)&st, sizeof(st));
	LeaveCriticalSection(&cs);
}

DWORD WINAPI serverThread(LPVOID pr_)
{
	PipeInfo pr = *(PipeInfo*)pr_;
	ConnectNamedPipe(pr.hReadHandle, (LPOVERLAPPED)NULL);
	ConnectNamedPipe(pr.hWriteHandle, (LPOVERLAPPED)NULL);
	command c;
	DWORD bytes;
	DWORD pos;
	do {
		ReadFile(pr.hReadHandle, (char*)&c, sizeof(c), &bytes, 0);
		if (c.type == 0) {
			employee st;
			c.result = lookUp(c.num, st, pos);
			WriteFile(pr.hWriteHandle, &c, sizeof(c), &bytes, 0);
			if (c.result) {
				countOfReaders[pos]++;
				WaitForSingleObject(hWriteAccess[pos], INFINITE);
				ResetEvent(hReadAccess[pos]);
				read(st, pos);
				WriteFile(pr.hWriteHandle, &st, sizeof(st), &bytes, 0);
				countOfReaders[pos]--;
				if (!countOfReaders[pos])
					SetEvent(hReadAccess[pos]);
			}
		}
		else if (c.type == 1) {
			employee st;
			c.result = lookUp(c.num, st, pos);
			WriteFile(pr.hWriteHandle, &c, sizeof(c), &bytes, 0);
			if (c.result) {
				WaitForSingleObject(hReadAccess[pos], INFINITE);
				WaitForSingleObject(hWriteAccess[pos], INFINITE);
				ResetEvent(hWriteAccess[pos]);
				read(st, pos);
				WriteFile(pr.hWriteHandle, &st, sizeof(st), &bytes, 0);

				ReadFile(pr.hReadHandle, (char*)&st, sizeof(st), &bytes, 0);
				write(st, pos);
				SetEvent(hWriteAccess[pos]);
			}

		}
	} while (c.type != 3);
	DisconnectNamedPipe(pr.hReadHandle);
	DisconnectNamedPipe(pr.hWriteHandle);
	return 0;
}

void print() {
	file.seekg(0, std::ios::beg);
	int N;
	file.read((char*)&N, sizeof(int));
	employee st;
	for (int i = 0; i < N; ++i) {
		file.read((char*)&st, sizeof(st));
		std::cout << "\t\n___Record number " << i + 1 << " ____\n";
		std::cout << "Enter employee's number: " << st.num;
		std::cout << "\nName: " << st.name;
		std::cout << "\Hours: " << st.hours << std::endl;
	}
}

int main() {
	filename = new char[MAXFILENAMELEN];
	init();
	print();
	int C;
	std::cout << "Enter count of client processes: ";
	std::cin >> C;
	InitializeCriticalSection(&cs);

	HANDLE* hThread = new HANDLE[C];
	DWORD* ThreadID = new DWORD[C];
	PipeInfo* allPipes = new PipeInfo[C];
	for (int i = 0; i < C; ++i) {
		char PipeName[40];
		sprintf(PipeName, "\\\\.\\pipe\\Pipe_%d_%d", 1, i);
		std::cout << PipeName << std::endl;
		allPipes[i].hReadHandle = CreateNamedPipe((LPWSTR)PipeName, PIPE_ACCESS_INBOUND, PIPE_TYPE_MESSAGE | PIPE_WAIT, 2, 0, 0, INFINITE, (LPSECURITY_ATTRIBUTES)NULL);
		sprintf(PipeName, "\\\\.\\pipe\\Pipe_%d_%d", 2, i);
		std::cout << PipeName << std::endl;
		allPipes[i].hWriteHandle = CreateNamedPipe((LPWSTR)PipeName, PIPE_ACCESS_OUTBOUND, PIPE_TYPE_MESSAGE | PIPE_WAIT, 2, 0, 0, INFINITE, (LPSECURITY_ATTRIBUTES)NULL);
		hThread[i] = CreateThread(NULL, 0, serverThread, (void*)&allPipes[i], 0, ThreadID + i);
		std::cout << "\nClient ID:" << i;
	}
	WaitForMultipleObjects(C, hThread, TRUE, INFINITE);
	delete[] filename;
	delete[] hThread;
	delete[] ThreadID;
	delete[] hWriteAccess;
	std::cout << "Final result:\n\n";
	print();
	file.close();
	return 0;
}