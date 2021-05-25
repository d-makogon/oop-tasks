#pragma once

#include <string>
#include <iostream>

class Console
{
public:
    struct PrintStyle
    {
        enum class ForegroundColor
        {
            Default = 0,
            White = 37,
            Black = 30,
            Red = 31,
            Green = 32,
            Yellow = 33,
            Blue = 34,
            Magenta = 35,
            Cyan = 36,
        };

        enum class BackgroundColor
        {
            Default = 0,
            Black = 40,
            Red = 41,
            Green = 42,
            Yellow = 43,
            Blue = 44,
            Magenta = 45,
            Cyan = 46,
            White = 47,
        };

        enum class TextStyle
        {
            None = 0,
            Bold = 1,
            Underline = 4,
            Inverse = 7,
        };

        ForegroundColor fgColor;
        BackgroundColor bgColor;
        TextStyle textStyle;

        friend std::ostream& operator<<(std::ostream& os, const Console::PrintStyle& style);
    };

    constexpr static const PrintStyle DefaultPrintStyle = {
            PrintStyle::ForegroundColor::Default,
            PrintStyle::BackgroundColor::Default,
            PrintStyle::TextStyle::None,
    };

    constexpr static const PrintStyle ErrorPrintStyle = {
            PrintStyle::ForegroundColor::Red,
            PrintStyle::BackgroundColor::Default,
            PrintStyle::TextStyle::Bold,
    };

    constexpr static const PrintStyle InfoPrintStyle = {
            PrintStyle::ForegroundColor::Yellow,
            PrintStyle::BackgroundColor::Default,
            PrintStyle::TextStyle::Bold,
    };

    constexpr static const PrintStyle OKPrintResult = {
            PrintStyle::ForegroundColor::Green,
            PrintStyle::BackgroundColor::Default,
            PrintStyle::TextStyle::Bold,
    };

    static int CharToInt(const char& c);

    static char IntToChar(const int& c);

    template<typename T>
    static void Print(const T& t)
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
    static void PrintColored(const T& t, const PrintStyle& style = DefaultPrintStyle)
    {
        std::cout << style << t << DefaultPrintStyle;
    }

    template<typename T>
    static void PrintColored(const T& t, const PrintStyle::ForegroundColor& fgColor)
    {
        auto style = DefaultPrintStyle;
        style.fgColor = fgColor;
        std::cout << style << t << DefaultPrintStyle;
    }

    template<typename... Args>
    static void
    PrintColoredFormatted(const std::string& s, const PrintStyle& style, const Args& ... args)
    {
        std::cout << style;
        PrintFormatted(s, args...);
        std::cout << DefaultPrintStyle;
    }

    template<typename... Args>
    static void
    PrintColoredFormatted(const std::string& s, const PrintStyle::ForegroundColor& fgColor, const Args& ... args)
    {
        auto style = DefaultPrintStyle;
        style.fgColor = fgColor;
        std::cout << style;
        PrintFormatted(s, args...);
        std::cout << DefaultPrintStyle;
    }

    template<typename... Args>
    static void PrintError(const std::string& s, const Args& ... args)
    {
        PrintColoredFormatted(s, ErrorPrintStyle, args...);
    }

    template<typename... Args>
    static void PrintInfo(const std::string& s, const Args& ... args)
    {
        PrintColoredFormatted(s, InfoPrintStyle, args...);
    }

    template<typename... Args>
    static void PrintOK(const std::string& s, const Args& ... args)
    {
        PrintColoredFormatted(s, OKPrintResult, args...);
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
