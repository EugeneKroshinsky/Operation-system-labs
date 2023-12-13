#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <conio.h>

#include <locale>
#include <codecvt>

int main()
{
    //создается creator
    std::string binaryFileName;
    int notesNumber;

    std::cout << "Enter binary file name" << std::endl;
    std::cin >> binaryFileName;

    std::cout << "Enter number of notes" << std::endl;
    std::cin >> notesNumber;

    std::string buf = (binaryFileName + " " + std::to_string(notesNumber)).c_str();
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring creatorArgument = converter.from_bytes(buf);
    wchar_t lpszAppName[] = L"D:\\ Visual Studio My Projects\\OSLab1\\x64\\Debug\\CreatorInFolder.exe";

    STARTUPINFO si;
    PROCESS_INFORMATION piApp;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    if (!CreateProcess(lpszAppName,(LPWSTR)creatorArgument.c_str(), NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
    {
        _cputs("The new process is not created.\n");
        _cputs("Check a name of the process.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return 0;
    }

    WaitForSingleObject(piApp.hProcess, INFINITE);
    CloseHandle(piApp.hThread);
    CloseHandle(piApp.hProcess);

    //вывод  содержимого бинарного файла
    std::cout << std::endl;
    std::cout << "Binary file:" << std::endl;
    std::ifstream fin(binaryFileName, std::ios::binary);
    employee e;
    while (fin.read((char*)&e, sizeof(employee))) {
        std::cout << e.num << " " << e.name << " " << e.hours << std::endl;
    }
    fin.close();
    std::cout << std::endl;

    //создается reporter
    std::string textFileName;
    double moneyPerHour;
    std::cout << "Enter text file name" << std::endl;
    std::cin >> textFileName;
    std::cout << "Enter money per hour" << std::endl;
    std::cin >> moneyPerHour;
    
    std::string buf2 = (binaryFileName + " " + textFileName + " " + std::to_string(moneyPerHour) + " " + std::to_string(notesNumber)).c_str();
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter2;
    std::wstring creatorArgument2 = converter2.from_bytes(buf2);
    wchar_t lpszAppName2[] = L"D:\\ Visual Studio My Projects\\OSLab1\\x64\\Debug\\ReporterInFolder.exe";
    STARTUPINFO si2;
    PROCESS_INFORMATION piApp2;

    ZeroMemory(&si2, sizeof(STARTUPINFO));
    si2.cb = sizeof(STARTUPINFO);

    if (!CreateProcess(lpszAppName2, (LPWSTR)creatorArgument2.c_str(), NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE, NULL, NULL, &si2, &piApp2))
    {
        _cputs("The new process is not created.\n");
        _cputs("Check a name of the process.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return 0;
    }

    WaitForSingleObject(piApp2.hProcess, INFINITE);
    CloseHandle(piApp2.hThread);
    CloseHandle(piApp2.hProcess);

    std::cout << std::endl;
    std::cout << "Text file:" << std::endl;
    std::ifstream fin2(textFileName);
    
    for (int i = 0; i < notesNumber; i++)
    {
        std::string line;
        getline(fin2, line);
        std::cout << line << std::endl;
    }
    fin2.close();
    return 0;
}