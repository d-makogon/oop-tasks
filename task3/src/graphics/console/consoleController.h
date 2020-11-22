#pragma once

#include <string>
#include <iostream>

class Console
{
public:
    enum class ForegroundColor
    {
        Black = 30,
        Red = 31,
        Green = 32,
        Yellow = 33,
        Blue = 34,
        Magenta = 35,
        Cyan = 36,
        White = 37
    };

    enum class BackgroundColor
    {
        Black = 40,
        Red = 41,
        Green = 42,
        Yellow = 43,
        Blue = 44,
        Magenta = 45,
        Cyan = 46,
        White = 47
    };

    enum class TextStyle
    {
        None = 0,
        Bold = 1,
        Underline = 4,
        Inverse = 7
    };

    static int LetterToCoord(const char& c);

    static char CoordToLetter(const int& c);

    template<typename T>
    static void Print(T t)
    {
        std::cout << t;
    }

    template<typename T, typename... Args>
    static void Print(const T& t, const Args& ... args)
    {
        std::cout << t;
        Print(args...);
    }

    template<typename T>
    static void PrintColored(const T& t,
                             Console::ForegroundColor fgColor = Console::ForegroundColor::White,
                             Console::BackgroundColor bgColor = Console::BackgroundColor::Black,
                             Console::TextStyle style = Console::TextStyle::None)
    {
        std::cout << "\033[" << static_cast<int>(style) << ";" << static_cast<int>(bgColor) << ";"
                  << static_cast<int>(fgColor) << "m" << t << "\033[0m";
    }

    template<typename... Args>
    static void
    PrintColoredFormatted(const std::string& s, Console::ForegroundColor fgColor, Console::BackgroundColor bgColor,
                          Console::TextStyle style, const Args& ... args)
    {
        std::cout << "\033[" << static_cast<int>(style) << ";" << static_cast<int>(bgColor) << ";"
                  << static_cast<int>(fgColor) << "m";
        PrintFormatted(s, args...);
        std::cout << "\033[0m";
    }


    template<typename... Args>
    static void PrintFormatted(const std::string& s, const Args& ... args)
    {
        printf(s.c_str(), args...);
    }

    static void PrintLine(const std::string& line = "");

    static void Clear();

    static std::string GetLine();
};
