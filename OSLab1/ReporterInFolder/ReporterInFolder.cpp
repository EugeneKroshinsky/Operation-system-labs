#include <iostream>
#include <fstream>

struct employee
{
	int num; // идентификационный номер сотрудника
	char name[10]; // имя сотрудника
	double hours; // количество отработанных часов
};

int main(int argc, char* argv[])
{
	std::ifstream fin(argv[0], std::ios::binary);
	std::ofstream fout(argv[1]);
	double moneyPerHour = atof(argv[2]);
	int notesNumber = atoi(argv[3]);
	
	for (int i = 0; i < notesNumber; i++)
	{
		employee e;
		fin.read((char*)&e, sizeof(employee));
		fout << e.num << " " << e.name << " " << e.hours << " " << e.hours * moneyPerHour << std::endl;
	}
	return 0;
}
