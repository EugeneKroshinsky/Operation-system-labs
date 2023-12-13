#include <fstream>
#include <iostream>
struct employee
{
    int num; // идентификационный номер сотрудника
    char name[10]; // имя сотрудника
    double hours; // количество отработанных часов
};

int main(int argc, char* argv[])
{
    std::ofstream fout(argv[0], std::ios::binary);
    int notesNumber = atoi(argv[1]);
    for (int i = 0; i < notesNumber; i++) {
        employee person;

        std::cout << "Enter employee num: " << std::endl;
        std::cin >> person.num;

        std::cout << "Enter employee name: " << std::endl;
        std::cin >> person.name;

        std::cout << "Enter employee hours: " << std::endl;
        std::cin >> person.hours;

        fout.write((char*)&person, sizeof(employee));
    }
}

