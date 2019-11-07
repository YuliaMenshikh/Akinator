#include "Text.h"

#include <sys/stat.h>
#include <cstdint>

Text::Text() : _text(nullptr), _textSize(0) {}

Text::Text(const char* fileName) : _textSize(0), _text(nullptr)
{
    _textSize = SizeOfText(fileName);

    FILE* file = nullptr;
    OpenFile(&file, fileName);

    ++_textSize;
    _text = new char[_textSize];
    fread(_text, sizeof(char), _textSize, file);
    _text[_textSize - 1] = '\0';

    fclose(file);
}

Text::~Text()
{
    delete[] _text;
}

char& Text::operator[](size_t ind) const
{
    return _text[ind];
}

size_t Text::Size() const
{
    return _textSize;
}

void Text::OpenFile(FILE **file, const char *fileName)
{
    while (*file == nullptr) {
        *file = fopen(fileName, "r");
        if (*file == nullptr) {
            printf("Wrong name of file");
        }
    }
}

int Text::SizeOfText(const char *fileName)
{
    struct stat file_stats;
    stat(fileName, &file_stats);
    return file_stats.st_size;
}

void Text::WriteToFile(const char* fileName, const char* text, int size)
{
    FILE* file = nullptr;
    file = fopen(fileName, "w");
    for (int i = 0; i < size; ++i)
    {
        fputc(*(text + i), file);
    }
    fclose(file);
}