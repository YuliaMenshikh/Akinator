#include "Akinator.h"
#include "Settings.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>

Akinator::Akinator()
{
    epilogue = '\"';
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
    strcat(new_buf, prologue);
    strcat(new_buf, ss);
    strcat(new_buf, "\"");
    system(new_buf);
}


void Akinator::Run()
{
    Node* current = _root;
    bool more = true;
    while(more)
    {
        if (!current->yes)
        {
            buffer = "Это был " + current->question + ". Не так ли?\n";
            write();
            std::cout << "1 - yes\n0 - no\n";
            bool guess = false;
            std::cin >> guess;

            if (guess)
            {
                buffer = "Урааа!!! Все-таки я была права\n";
                write();
            }
            else
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
                    char c;
                    while ((c = getchar()) != '\n')
                    {
                        newQuestion += c;
                    }
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
            buffer = "Вы хотите сыграть еще?\n";
            write();
            std::cout << "1 - yes\n0 - no\n";
            std::cin >> more;
            current = _root;
        }
        else
        {
            buffer = current->question + "?\n";
            write();
            std::cout << "1 - yes\n0 - no\n";
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
                def.push_back("не " + current->question);
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

void Akinator::Compare(const std::string& first, const std::string& second)
{
    Node *node1, *node2;
    if (_characters.count(first))
    {
        node1 = _characters[first];
    }
    else
    {
        buffer = "Нет персонажа " + first + '\n';
        write();
        return;
    }
    if (_characters.count(second))
    {
        node2 = _characters[second];
    }
    else
    {
        buffer = "Нет персонажа " + second + '\n';
        write();
        return;
    }

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

    while (hight1 != hight2)
    {
        if (hight1 > hight2)
        {
            hight1--;
            bool comeBy = node1->comeBy;
            node1 = node1->parent;
            if (comeBy)
                def1.push_back(node1->question);
            else
                def1.push_back("не " + node1->question);

        }
        else
        {
            hight2--;
            bool comeBy = node2->comeBy;
            node2 = node2->parent;
            if (comeBy)
                def2.push_back(node2->question);
            else
                def2.push_back("не " + node2->question);
        }
    }

    while (node1 != node2)
    {
        bool comeBy1 = node1->comeBy;
        bool comeBy2 = node2->comeBy;
        node1 = node1->parent;
        node2 = node2->parent;
        if (comeBy1)
            def1.push_back(node1->question);
        else
            def1.push_back("не " + node1->question);

        if (comeBy2)
            def2.push_back(node2->question);
        else
            def2.push_back("не " + node2->question);
    }
    while (node1->parent)
    {
        bool comeBy = node1->comeBy;
        node1 = node1->parent;
        if (comeBy)
            def.push_back(node1->question);
        else
            def.push_back("не " + node1->question);
    }


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

void Akinator::WriteToDotFile(const char *fileOut)
{
    FILE* file = nullptr;
    file = fopen(fileOut, "w");
    fputs("Digraph{\n", file);

    DfsWriteDot(_root, file);

    fputc('}', file);
    fclose(file);
}

void Akinator::ShowDotFile()
{
    WriteToDotFile("tree.dot");
    system("dot tree.dot -Tpng -o tree.png");
    system("open tree.png ");
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
        fputc('"', file);
        for (char i : node->yes->question)
            fputc(i, file);
        fputc('"', file);
        fputs("[style = filled, fillcolor = darkolivegreen1];\n", file);

        fputc('"', file);
        for (char i : node->question)
            fputc(i, file);
        fputc('"', file);
        fputs("->", file);
        fputc('"', file);
        for (char i : node->yes->question)
            fputc(i, file);
        fputc('"', file);
        fputs("[color = forestgreen]", file);
        fputs(";\n", file);

        fputc('"', file);
        for (char i : node->no->question)
            fputc(i, file);
        fputc('"', file);
        fputs("[style = filled, fillcolor = indianred1];\n", file);

        fputc('"', file);
        for (char i : node->question)
            fputc(i, file);
        fputc('"', file);
        fputs("->", file);
        fputc('"', file);
        for (char i : node->no->question)
            fputc(i, file);
        fputc('"', file);
        fputs("[color = indianred1];\n", file);

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