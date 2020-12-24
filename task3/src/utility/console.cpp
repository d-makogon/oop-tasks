#include "include/console.h"

#include <string>
#include <tuple>
#include <iostream>

int Console::CharToInt(const char& c)
{
    return tolower(c) - 'a';
}

char Console::IntToChar(const int& c)
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
    std::cout << u8"\033[2J\033[1;1H";
#elif defined (__APPLE__)
    system("clear");
#endif
}

std::ostream& operator<<(std::ostream& os, const Console::PrintStyle& style)
{
    os << "\033[";
    os << static_cast<int>(style.textStyle) << ";";
    if (style.bgColor != Console::PrintStyle::BackgroundColor::Default)
        os << static_cast<int>(style.bgColor) << ";";
    if (style.fgColor != Console::PrintStyle::ForegroundColor::Default)
        os << static_cast<int>(style.fgColor);
    os << "m";
    return os;
}
