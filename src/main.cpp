#include <iostream>
#include "Akinator.h"

void ClientCode()
{
    Akinator* akinator = new Akinator("tree.txt");
    bool more = true;
    std::cout << "Привет) Я умею угадывать различных персонажей. Сыграем?)\n";
    while (more)
    {
        std::cout << "Выберете режим игры:\n0 - закончить\n1 - угадывание\n2 "
                     "- дать определение\n3 - сравнить двух персонажей\n";
        int command;
        std::cin >> command;
        if (command == 1)
        {
            akinator->Run();
        }
        else if (command == 2)
        {
            std::cout << "Введите персонажа: ";
            std::string name;
            std::cin >> name;
            akinator->Definition(name);
        }
        else if (command == 3)
        {
            std::cout << "Введите двух персонажей которых хотите сравнить: ";
            std::string first, second;
            std::cin >> first  >> second;
            akinator->Compare(first, second);
        }
        else if (command)
        {
            std::cout << "Такой команды я не предлагал\n";
        }
        else if (!command)
        {
            more = false;
        }
        if (command)
        {
            std::cout << "Хотите выбрать режим и сыграть еще?\n1 - yes\n0 - no\n";
            std::cin >> more;
        }
    }
    delete akinator;
}

int main()
{
    ClientCode();
    return 0;
}