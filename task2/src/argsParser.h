#pragma once
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class ArgsParser
{
public:
    ArgsParser(int& argc, char** argv);

    [[nodiscard]] const string& GetCmdOption(const string& option) const;

    [[nodiscard]] bool CmdOptionExists(const string& option) const;

private:
    vector<string> tokens;
};