#include <iostream>
#include "Akinator.h"

void ClientCode()
{
    Akinator* akinator = new Akinator("tree.txt");
    bool more = true;
    akinator->ShowDotFile();
    std::cout << "Привет) Я умею угадывать различных персонажей. Сыграем?)\n";
    system("say -v Milena \"Привет) Я умею угадывать различных персонажей. Сыграем?)\"");
    while (more)
    {
        std::cout << "Выберете режим игры:\n0 - закончить\n1 - угадывание\n2 "
                     "- дать определение\n3 - сравнить двух персонажей\n4 - показать дерево\n";
        system("say -v Milena \"Выберете режим игры. \"");
        int command;
        std::cin >> command;
        if (command == 1)
        {
            akinator->Run();
        }
        else if (command == 2)
        {
            std::cout << "Введите персонажа: \n";
            system("say -v Milena \"Введите персонажа. \"");
            std::string name;
            std::cin >> name;
            akinator->Definition(name);
        }
        else if (command == 3)
        {
            std::cout << "Введите двух персонажей которых хотите сравнить: \n";
            system("say -v Milena \"Введите двух персонажей которых хотите сравнить. \"");
            std::string first, second;
            std::cin >> first  >> second;
            akinator->Compare(first, second);
        }
        else if (command == 4)
        {
            akinator->ShowDotFile();
        }
        else if (command)
        {
            std::cout << "Такой команды я не предлагала\n";
            system("say -v Milena \"Такой команды я не предлагала\"");
        }
        else if (!command)
        {
            more = false;
        }
        if (command)
        {
            std::cout << "Хотите выбрать режим и сыграть еще?\n1 - yes\n0 - no\n";
            system("say -v Milena \"Хотите выбрать режим и сыграть еще?\"");
            std::cin >> more;
        }
    }
    system("say -v Milena \"Мне было очень интересно с Вами. До скорых встреч!\"");
    delete akinator;
}

int main()
{
    ClientCode();
    return 0;
}