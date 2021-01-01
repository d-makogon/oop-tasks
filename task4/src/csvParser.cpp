#include "../include/csvParser.h"

#include <memory>
#include <string>

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
    int size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
    if (size <= 0) { return ""; }
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1);
}

CSVParsingException::CSVParsingException(const std::string& msg, const std::string& csvLine, size_t errorPos,
                                         size_t lineNumber)
{
    if (csvLine.empty())
        whatMsg = std::make_unique<std::string>(
                string_format("Error: Line %d, Column %d: %s", lineNumber, errorPos, msg.c_str()));
    else
        whatMsg = std::make_unique<std::string>(
                string_format("Error: Line %d, Column %d: %s\nSource line:\n%s", lineNumber, errorPos, msg.c_str(),
                              csvLine.c_str()));
}

const char* CSVParsingException::what() const noexcept
{
    return whatMsg->c_str();
}
