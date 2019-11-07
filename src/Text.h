#pragma once

#include <stdio.h>

class Text
{
public:
    Text();
    Text(const char* fileName);
    ~Text();

    size_t Size() const;

    char& operator[](size_t ind) const;

    void WriteToFile(const char* fileName, const char* text, int size);
private:
    int SizeOfText(const char* fileName);
    void OpenFile(FILE** file, const char* fileName);

    char* _text;
    size_t _textSize;
};