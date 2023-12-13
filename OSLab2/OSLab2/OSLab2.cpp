#include <iostream>
#include <Windows.h>

struct parametr {
    parametr(int* arr, int n)
    {
        this->arr = arr;
        this->n = n;
    };
    int* arr;
    int min = 0;
    int max = 0;
    int average = 0;
    int n;
};

int* scanArray(int n) {
    std::cout << "Enter array" << std::endl;
    int* arr = new int[n];
    for (int i = 0; i < n; i++)
    {
        std::cin >> arr[i];
    }
    return arr;
}

void printArray(int n, int* arr) {
    std::cout << "Array:" << std::endl;
    for (int i = 0; i < n; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

CRITICAL_SECTION cs;
DWORD WINAPI min_max(LPVOID arg)
{
    parametr* array = (parametr*)arg;
    int max = INT_MIN;
    int min = INT_MAX;
    for (int i = 0; i < array->n; i++)
    {
        if (array->arr[i] > max)
        {
            max = array->arr[i];
        }
        Sleep(7);
        if (array->arr[i] < min)
        {
            min = array->arr[i];
        }
        Sleep(7);
    }
    array->min = min;
    array->max = max;
    EnterCriticalSection(&cs);
    std::cout << "Max: " << array->max << std::endl;
    std::cout << "Min: " << array->min << std::endl;
    LeaveCriticalSection(&cs);
    return 0;
}

DWORD WINAPI average(LPVOID arg) 
{
    parametr* array = (parametr*)arg;

    int sum = 0;
    for (int i = 0; i < array->n; i++)
    {
        sum += array->arr[i];
        Sleep(12);
    }
    array->average = sum / array->n;
    EnterCriticalSection(&cs);
    std::cout << "Average: " << array->average << std::endl;
    LeaveCriticalSection(&cs);
    return 0;
}

int main()
{
    int n;
    std::cout << "Enter n: " << std::endl;
    std::cin >> n;
    InitializeCriticalSection(&cs);
    int* arr = scanArray(n);
    printArray(n, arr);

    parametr* array = new parametr(arr, n);

    HANDLE hThread_min_max, hThread_average;

    hThread_min_max = CreateThread(NULL, 0, min_max, LPVOID(array), NULL, NULL);
    hThread_average = CreateThread(NULL, 0, average, LPVOID(array), NULL, NULL);
    if (hThread_min_max == NULL || hThread_average == NULL) {
        return GetLastError();
    }

    WaitForSingleObject(hThread_min_max, INFINITE);
    WaitForSingleObject(hThread_average, INFINITE);
    DeleteCriticalSection(&cs);
    CloseHandle(hThread_min_max);
    CloseHandle(hThread_average);

    std::cout << std::endl;

    for (int i = 0; i < array->n; ++i) {
        if (array->arr[i] == array->max || array->arr[i] == array->min) 
        {
            array->arr[i] = array->average;
        }
    }

    printArray(array->n, array->arr);
    std::cout << std::endl;

    delete[] arr;
    delete array;

    return 0;
}