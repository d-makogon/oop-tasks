#pragma once
#include <string>
#include <vector>
#include <algorithm>

class ArgsParser
{
public:
    ArgsParser(int& argc, char** argv);

    [[nodiscard]] const std::string& GetCmdOption(const std::string& option) const;

    [[nodiscard]] bool CmdOptionExists(const std::string& option) const;

private:
    std::vector<std::string> tokens;
};