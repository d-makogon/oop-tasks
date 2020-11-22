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
    std::getline(std::cin, s);
    return s;
}

void Console::Clear()
{
#if defined _WIN32
#include <conio.h>
    clrscr();
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
    // std::cout<< u8"\033[2J\033[1;1H";
#elif defined (__APPLE__)
    system("clear");
#endif
}

