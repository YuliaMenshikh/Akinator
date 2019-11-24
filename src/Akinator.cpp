#include "Akinator.h"
#include "Settings.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>

Akinator::Akinator()
{
    _root = new Node("никто", nullptr, false);
    _characters[_root->question] = _root;
}

Akinator::Akinator(const char* fileName, const char* dotFileName) :
                        text(fileName),
                        fileName(fileName),
                        dotFileName(dotFileName)
{
    ParseText(text);
}

Akinator::~Akinator()
{
    WriteToDotFile(dotFileName);
    WriteToBaseFile(fileName);
    Delete(_root);
}

void Akinator::write()
{
    std::cout << buffer;
    const char* ss = buffer.c_str();
    char new_buf[1024] = "";
    strcat(new_buf, prologue_to_show_tree);
    strcat(new_buf, ss);
    strcat(new_buf, epilogue_to_show_tree);
    system(new_buf);
}


void Akinator::Run()
{
    Node* current = _root;
    bool more = true;
    while (more)
    {
        if (!current->yes)
        {
            buffer = "Это был " + current->question + ". Не так ли?\n";
            write();
            std::cout << standart_choice;
            bool guess = false;
            std::cin >> guess;

            if (guess)
            {
                buffer = "Урааа!!! Все-таки я была права.\n";
                write();
            }
            else
            {
                NotGuessCase(current);
            }
            buffer = "Вы хотите сыграть еще?\n";
            write();
            std::cout << standart_choice;
            std::cin >> more;
            current = _root;
        }
        else
        {
            buffer = current->question + "?\n";
            write();
            std::cout << standart_choice;
            bool answer = false;
            std::cin >> answer;
            if (answer)
            {
                current = current->yes;
            }
            else
            {
                current = current->no;
            }
        }
    }
}

void Akinator::NotGuessCase(Node *current)
{
    buffer = "Какая жалость. Кто же это был?\n";
    write();
    std::string newPearson;
    std::cin >> newPearson;
    if (!_characters.count(newPearson))
    {
        buffer = "Чем же " + current->question + " отличается от " + newPearson + "?\n";
        write();
        std::string newQuestion;
        getchar();
        getline(std::cin, newQuestion);
        current->Split(newQuestion, newPearson);
        _characters[current->yes->question] = current->yes;
        _characters[current->no->question] = current->no;
    }
    else
    {
        buffer = "Такой персонаж уже есть. Вот его определение:\n";
        write();
        Definition(newPearson);
    }
}

void Akinator::Definition(const std::string& name)
{
    std::vector<std::string> def;
    if (_characters.count(name)) {
        Node *current = _characters[name];
        bool comeBy;
        while(current->parent)
        {
            comeBy = current->comeBy;
            current = current->parent;
            if (comeBy)
                def.push_back(current->question);
            else
                def.push_back(negition + current->question);
        }
        std::cout << "Определение " << name << ": ";
        for (int i = def.size() - 1; i >= 0; --i)
        {
            std::cout << def[i] << ", ";
        }
        std::cout << '\n';

    }
    else
    {
        buffer = "Нет такого персонажа\n";
        write();
    }
}

Node* Akinator::GetCharacter(const std::string &character, bool& NoCharacter)
{
    if (_characters.count(character))
    {
        return _characters[character];
    }
    else
    {
        buffer = "Нет персонажа " + character + '\n';
        write();
        NoCharacter = true;
        return nullptr;
    }
}

void Akinator::PrintDefinition(const std::string& first, const std::string& second, std::vector<std::string> &def,
        std::vector<std::string> &def1, std::vector<std::string> &def2)
{
    std::cout << "Общее: ";
    for (int i = def.size() - 1; i >= 0; --i)
    {
        std::cout << def[i] << ", ";
    }
    std::cout << '\n' << first << ": ";
    for (int i = def1.size() - 1; i >= 0; --i)
    {
        std::cout << def1[i] << ", ";
    }
    std::cout << '\n' << second << ": ";
    for (int i = def2.size() - 1; i >= 0; --i)
    {
        std::cout << def2[i] << ", ";
    }
    std::cout << '\n';
}

void Akinator::GoToOneLevel(Node **node, std::vector<std::string> &def, int& heightToChange, int height)
{
    while (heightToChange != height)
    {
        heightToChange--;
        bool comeBy = (*node)->comeBy;
        (*node) = (*node)->parent;
        if (comeBy)
            def.push_back((*node)->question);
        else
            def.push_back(negition + (*node)->question);
    }

}

void Akinator::Compare(const std::string& first, const std::string& second)
{
    bool NoCharacter = false;
    Node *node1 = GetCharacter(first, NoCharacter), *node2 = GetCharacter(second, NoCharacter);
    if (NoCharacter)
        return;

    int hight1 = 1, hight2 = 1;
    while (node1->parent)
    {
        hight1++;
        node1 = node1->parent;
    }
    while (node2->parent)
    {
        hight2++;
        node2 = node2->parent;
    }
    node1 = _characters[first];
    node2 = _characters[second];

    std::vector<std::string> def, def1, def2;

    if (hight1 > hight2)
        GoToOneLevel(&node1, def1, hight1, hight2);
    else if (hight2 > hight1)
        GoToOneLevel(&node2, def2, hight2, hight1);

    while (node1 != node2)
    {
        bool comeBy1 = node1->comeBy;
        bool comeBy2 = node2->comeBy;
        node1 = node1->parent;
        node2 = node2->parent;
        if (comeBy1)
            def1.push_back(node1->question);
        else
            def1.push_back(negition + node1->question);

        if (comeBy2)
            def2.push_back(node2->question);
        else
            def2.push_back(negition + node2->question);
    }
    while (node1->parent)
    {
        bool comeBy = node1->comeBy;
        node1 = node1->parent;
        if (comeBy)
            def.push_back(node1->question);
        else
            def.push_back(negition + node1->question);
    }

    PrintDefinition(first, second, def, def1, def2);
}

void Akinator::WriteToDotFile(const char *fileOut)
{
    FILE* file = nullptr;
    file = fopen(fileOut, "w");
    fputs(prologue_Digraph, file);

    DfsWriteDot(_root, file);

    fputc(close_delim, file);
    fclose(file);
}

void Akinator::ShowDotFile()
{
    WriteToDotFile(dotFileName);
    system("dot tree.dot -Tpng -o tree.png");
    system("open tree.png");
}

void Akinator::WriteToBaseFile(const char *fileOut)
{
    FILE* file = nullptr;
    file = fopen(fileOut, "w");
    fputc(open_delim, file);

    DfsWriteBase(_root, file);

    fputc(close_delim, file);
    fclose(file);
}

void Akinator::DfsWriteBase(Node* node, FILE* file)
{
    fputc(symbol_of_string, file);
    for (char i : node->question)
        fputc(i, file);
    fputc(symbol_of_string, file);

    if (node->yes)
    {
        fputc(open_delim, file);
        DfsWriteBase(node->yes, file);
        fputc(close_delim, file);
        DfsWriteBase(node->no, file);
    }
}

void Akinator::DfsWriteDot(Node* node, FILE* file)
{
    if (node->yes)
    {
        fputc(symbol_of_string, file);
        fputs(node->yes->question.c_str(), file);
        fputc(symbol_of_string, file);
        fputs(filled_green, file);

        fputc(symbol_of_string, file);
        fputs(node->question.c_str(), file);
        fputc(symbol_of_string, file);
        fputs(arrow, file);
        fputc(symbol_of_string, file);
        fputs(node->yes->question.c_str(), file);
        fputc(symbol_of_string, file);
        fputs(end_of_str, file);

        fputc(symbol_of_string, file);
        fputs(node->no->question.c_str(), file);
        fputc(symbol_of_string, file);
        fputs(filled_red, file);

        fputc(symbol_of_string, file);
        fputs(node->question.c_str(), file);
        fputc(symbol_of_string, file);
        fputs(arrow, file);
        fputc(symbol_of_string, file);
        fputs(node->no->question.c_str(), file);
        fputc(symbol_of_string, file);
        fputs(end_of_str, file);

        DfsWriteDot(node->yes, file);
        DfsWriteDot(node->no, file);
    }
}

int Akinator::ParseText(const Text& text)
{
    if (!text.Size())
        return 1;
    int ind = 0;
    while (ind < text.Size() && text[ind] != open_delim )
    {
        ind++;
    }
    if (text[ind] != open_delim)
        return 1;
    ind++;

    _root = ReadNode(text, ind, nullptr, 0);
    return 0;
}

Node* Akinator::ReadNode(const Text& text, int& ind, Node* parent, bool comeBy)
{
    while (ind < text.Size() && text[ind] != symbol_of_string)
        ind++;
    ind++;

    std::string question;

    char c;
    while (ind < text.Size() && text[ind] != symbol_of_string)
    {
        c = text[ind];
        question += c;
        ind++;
    }

    ind++;
    Node* currentNode = new Node(question, parent, comeBy);

    while (ind < text.Size() && (text[ind] != open_delim && text[ind] != close_delim))
        ind++;

    if (text[ind] == open_delim)
    {
        currentNode->yes = ReadNode(text, ind, currentNode, true);
        while (ind < text.Size() && text[ind] != symbol_of_string)
            ind++;
        currentNode->no = ReadNode(text, ind, currentNode, false);
    }
    else
    {
        _characters[question] = currentNode;
    }

    return currentNode;
}

void Akinator::Delete(Node *node)
{
    if (node->yes)
    {
        Delete(node->yes);
        Delete(node->no);
    }
    delete node;
}