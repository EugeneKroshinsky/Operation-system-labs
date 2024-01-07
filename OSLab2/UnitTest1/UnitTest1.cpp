#include "pch.h"
#include "CppUnitTest.h"
#include <windows.h>
#include <vector>
#include <algorithm>
#include <numeric>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

struct parametr
{
	double* arr;
	int n;
	double average;
	double min_;
	double max_;
	parametr(double* a_, int size_) : arr(a_), n(size_), average(0), min_(0), max_(0) {}
};

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
	array->min_ = min;
	array->max_ = max;
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
	return 0;
}


namespace UnitTest
{
	TEST_CLASS(UnitTestMinMax)
	{
	public:

		TEST_METHOD(TestMethodMinMaxAuto)
		{
			srand(time(NULL));
			int size = rand() % 10;
			std::vector<double> test(size);
			double* a = new double[size];
			for (int i = 0; i < size; ++i)
			{
				a[i] = test[i] = rand() % 100;
			}
			auto param = new parametr(a, size);
			min_max((LPVOID)param);
			double min = *std::min_element(test.begin(), test.end());
			double max = *std::max_element(test.begin(), test.end());
			Assert::IsTrue(abs(min - param->min_) < 1e-5);
			Assert::IsTrue(abs(max - param->max_) < 1e-5);
		}

		TEST_METHOD(TestMethodMinMaxManual)
		{
			double* a = new double[5] {2, 4, 3, 5, 8};
			auto param = new parametr(a, 5);
			min_max((LPVOID)param);
			Assert::IsTrue(abs(2 - param->min_) < 1e-5);
			Assert::IsTrue(abs(8 - param->max_) < 1e-5);
		}
	};

	TEST_CLASS(UnitTestAverage)
	{
	public:

		TEST_METHOD(TestMethodAverageAuto)
		{
			double* test = new double[5] {5,10,15,20,25};
			auto param = new parametr(test, 5);
			average((LPVOID)param);
			Assert::IsTrue(abs(15 - param->average) == 0);
		}

		TEST_METHOD(TestMethodAverageManual)
		{
			double* test = new double[5] {2, 4, 6, 8, 10};
			auto param = new parametr(test, 5);
			average((LPVOID)param);
			Assert::IsTrue(abs(6 - param->average) == 0);
		}
	};
}
