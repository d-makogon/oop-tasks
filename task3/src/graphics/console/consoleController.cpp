#include "consoleController.h"

#include <string>
#include <iostream>

int Console::LetterToCoord(const char& c)
{
    return tolower(c) - 'a';
}

char Console::CoordToLetter(const int& c)
{
    return tolower(c) + 'a';
}

void Console::PrintLine(const std::string& line)
{
    std::cout << line << std::endl;
}

std::string Console::GetLine()
{
    std::string s;
    std::cin >> s;
    return s;
}
