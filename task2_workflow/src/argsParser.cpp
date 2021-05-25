#include <argsParser.h>

#include <string>

using namespace std;

ArgsParser::ArgsParser(int& argc, char** argv)
{
    for (int i = 1; i < argc; i++)
    {
        this->tokens.emplace_back(argv[i]);
    }
}

const string& ArgsParser::GetCmdOption(const string& option) const
{
    auto itr = find(this->tokens.begin(), this->tokens.end(), option);

    if (itr != this->tokens.end() && (itr + 1) != this->tokens.end())
    {
        return *(itr + 1);
    }

    static const string empty_string;
    return empty_string;
}

bool ArgsParser::CmdOptionExists(const string& option) const
{
    return find(this->tokens.begin(), this->tokens.end(), option)
           != this->tokens.end();
}
