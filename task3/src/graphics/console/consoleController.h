#pragma once

#include <string>
#include <iostream>

class Console
{
public:
    static int LetterToCoord(const char& c);

    static char CoordToLetter(const int& c);

    template<typename T>
    static void Print(T t)
    {
        std::cout << t;
    }

    template<typename T, typename... Args>
    static void Print(T t, Args... args)
    {
        std::cout << t;
        Print(args...);
    }

    template<typename... Args>
    static void PrintFormatted(const std::string& s, Args... args)
    {
        printf(s.c_str(), args...);
    }

    static void PrintLine(const std::string& line = "");

    static std::string GetLine();
};
