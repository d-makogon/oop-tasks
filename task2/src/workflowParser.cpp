#include <workflowParser.h>

#include <istream>
#include <fstream>
#include <regex>
#include <memory>
#include <unordered_map>

#include <iostream>
#include <utility>

#include <worker.h>
#include <workers.h>
#include <workersFactory.h>

using namespace std;

bool str2size_t(size_t& n, const string& str)
{
    const char* s = str.c_str();
    char* end;
    unsigned long long l;
    errno = 0;
    l = strtoull(s, &end, 10);
    if ((errno == ERANGE && l == numeric_limits<unsigned long long>::max()) || l > numeric_limits<size_t>::max())
    {
        return false;
    }
    if ((errno == ERANGE && l == numeric_limits<unsigned long long>::min()) || l < numeric_limits<size_t>::min())
    {
        return false;
    }
    if (*s == '\0' || *end != '\0')
    {
        return false;
    }
    n = l;
    return true;
}

WorkflowParser::WorkflowParser(string filename, Factory<Worker> factory)
{
    _filename = std::move(filename);
    _factory = std::move(factory);

    _inputStream = ifstream();
    _inputStream.exceptions(ifstream::failbit | ifstream::badbit);

    try
    {
        _inputStream.open(_filename);
    }
    catch (ifstream::failure& e)
    {
        throw WorkflowException({"Error opening file ", _filename, "\n"});
    }
}

vector<string> splitString(string s, char delimiter)
{
    vector<string> substrings;

    size_t delimIndex;
    while (string::npos != (delimIndex = s.find(delimiter)))
    {
        string subs = s.substr(0, delimIndex);
        s.erase(0, delimIndex + 1);
        substrings.emplace_back(subs);
    }
    if (!s.empty())
    {
        substrings.emplace_back(s);
    }
    return substrings;
}

shared_ptr<WorkflowParser::TWorkersMap> WorkflowParser::ParseWorkers()
{
    string s;

    try
    {
        getline(_inputStream, s);
    }
    catch (ifstream::failure& e)
    {
        throw WorkflowException({"Error reading from file ", _filename});
    }
    if (s != "desc")
    {
        throw WorkflowException("File doesn't start with 'desc'\n");
    }

    auto workers = make_shared<WorkflowParser::TWorkersMap>();
    std::regex workersRegex("^([0-9]+) = (readfile|writefile|grep|sort|replace|dump)(?: (.+)$|$)");
    std::smatch workersSmatch;

    try
    {
        while (!_inputStream.eof() && getline(_inputStream, s))
        {
            if (regex_search(s, workersSmatch, workersRegex))
            {
                // $1 - id
                // $2 - command name
                // $3 - args

                size_t id;
                if (!str2size_t(id, workersSmatch[1].str()))
                {
                    throw WorkflowException({"Error parsing number (in line ", s, ")\n"});
                }

                if (0 != workers->count(id))
                {
                    throw WorkflowException("Duplicate ids in blocks description\n");
                }

                vector<string> args = splitString(workersSmatch[3], ' ');

                auto worker = _factory.Create(workersSmatch[2], args);
                if (worker == nullptr)
                {
                    throw WorkflowException({"Error parsing worker name \"", workersSmatch[2], "\"\n"});
                }
                (*workers)[id] = worker;
            }
            else if (s == "csed")
            {
                break;
            }
            else
            {
                throw WorkflowException({"Error parsing line \"", s, "\"\n"});
            }
        }
    }
    catch (ifstream::failure& e)
    {
        throw WorkflowException({"Error opening/reading file ", _filename});
    }
    return workers;
}

vector<size_t> WorkflowParser::ParseExecutionOrder()
{
    string s;
    try
    {
        getline(_inputStream, s);
    }
    catch (ifstream::failure& e)
    {
        throw WorkflowException({"Error reading execution order line from ", _filename, "\n"});
    }

    string afterS;
    try
    {
        getline(_inputStream, afterS);
        // in case last line (containing exec order) is terminated with \n
        if (!afterS.empty())
        {
            throw ifstream::failure("");
        }
    }
    catch (ifstream::failure& e)
    {
        if (!afterS.empty())
        {
            throw WorkflowException({"Error parsing line \"", afterS, "\"\n"});
        }
    }

    vector<size_t> executionOrder;

    regex idRegex("^([0-9]+)( -> |$)");
    smatch idSmatch;
    bool hasArrow = false;

    while (regex_search(s, idSmatch, idRegex))
    {
        size_t id;
        if (!str2size_t(id, idSmatch[1].str()))
        {
            throw WorkflowException({"Error parsing number (in line ", s, ")\n"});
        }

        hasArrow = false;
        if (" -> " == idSmatch[2].str())
        {
            hasArrow = true;
        }

        executionOrder.push_back(id);
        s = idSmatch.suffix().str();
    }
    if (!s.empty() || hasArrow)
    {
        throw WorkflowException("Error reading execution order line\n");
    }

    return executionOrder;
}
