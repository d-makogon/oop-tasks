#include "workflowParser.h"

#include <istream>
#include <fstream>
#include <sstream>
#include <regex>
#include <memory>
#include <unordered_map>

#include <iostream>

#include "worker.h"
#include "workers.h"

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

Worker* GetWorkerByName(size_t id, const string& workerName, const string& args)
{
    if ("readfile" == workerName)
    {
        if (args.empty())
        {
            stringstream ss;
            ss << "Couldn't parse arguments for id" << id << endl;
            throw WorkflowParsingException(ss.str());
        }
        return new ReadFileWorker(id, args);
    }
    if ("writefile" == workerName)
    {
        if (args.empty())
        {
            stringstream ss;
            ss << "Couldn't parse arguments for id" << id << endl;
            throw WorkflowParsingException(ss.str());
        }
        return new WriteFileWorker(id, args);
    }
    if ("grep" == workerName)
    {
        if (args.empty())
        {
            stringstream ss;
            ss << "Couldn't parse arguments for id" << id << endl;
            throw WorkflowParsingException(ss.str());
        }
        return new GrepWorker(id, args);
    }
    if ("sort" == workerName)
    {
        if (!args.empty())
        {
            stringstream ss;
            ss << "Couldn't parse arguments for id" << id << endl;
            throw WorkflowParsingException(ss.str());
        }
        return new SortWorker(id);
    }
    if ("dump" == workerName)
    {
        if (args.empty())
        {
            stringstream ss;
            ss << "Couldn't parse arguments for id" << id << endl;
            throw WorkflowParsingException(ss.str());
        }
        return new DumpWorker(id, args);
    }
    if ("replace" == workerName)
    {
        string argsCopy = args;
        string word1 = argsCopy.substr(0, argsCopy.find(' '));
        argsCopy.erase(0, argsCopy.find(' ') + 1);
        string word2 = argsCopy.substr(0, argsCopy.find(' '));
        if (word1.empty() || word2.empty())
        {
            stringstream ss;
            ss << "Couldn't parse arguments for id" << id << endl;
            throw WorkflowParsingException(ss.str());
        }
        return new ReplaceWorker(id, word1, word2);
    }
    stringstream ss;
    ss << "Invalid worker name: " << workerName << endl;
    throw WorkflowParsingException(ss.str());
}

WorkflowParser::WorkflowParser(const string& filename)
{
    _filename = filename;

    _inputStream = ifstream();
    _inputStream.exceptions(ifstream::failbit | ifstream::badbit);

    try
    {
        _inputStream.open(filename);
    }
    catch (ifstream::failure& e)
    {
        stringstream ss;
        ss << "Error opening file " << filename << endl;
        throw WorkflowParsingException(ss.str());
    }
}

shared_ptr<unordered_map<size_t, Worker*>> WorkflowParser::ParseWorkers()
{
    string s;

    try
    {
        getline(_inputStream, s);
    }
    catch (ifstream::failure& e)
    {
        stringstream ss;
        ss << "Error reading from file " << _filename << endl;
        throw WorkflowParsingException(ss.str());
    }
    if (s != "desc")
    {
        throw WorkflowParsingException("File doesn't start with 'desc'\n");
    }

    auto workers = make_unique<unordered_map<size_t, Worker*>>();
    regex blocksRegex("^id([0-9]+) = (readfile|writefile|grep|sort|replace|dump)(?: (.+)$|$)");
    smatch blocksSm;

    try
    {
        while (!_inputStream.eof() && getline(_inputStream, s))
        {
            if (regex_search(s, blocksSm, blocksRegex))
            {
                // $1 - id
                // $2 - command name
                // $3 - args

                size_t id;
                if (!str2size_t(id, blocksSm[1].str()))
                {
                    stringstream ss;
                    ss << "Error parsing number (in line " << s << ")\n";
                    throw WorkflowParsingException(ss.str());
                }

                if (0 != workers->count(id))
                {
                    throw WorkflowParsingException("Duplicate ids in blocks description\n");
                }

                (*workers)[id] = GetWorkerByName(id, blocksSm[2], blocksSm[3]);
            }
            else if (s == "csed")
            {
                break;
            }
            else
            {
                stringstream ss;
                ss << "Error parsing line '" << s << "'\n";
                throw WorkflowParsingException(ss.str());
            }
        }
    }
    catch (ifstream::failure& e)
    {
        stringstream ss;
        ss << "Error opening/reading file " << _filename << endl;
        throw WorkerExecutionException(ss.str());
    }
    return workers;
}

vector<size_t> WorkflowParser::ParseExecutionOrder()
{
    string s;
    string afterS;
    try
    {
        getline(_inputStream, s);

        // in case last line (containing exec order) is terminated with \n:
        if (!_inputStream.eof())
        {
            getline(_inputStream, afterS);
            if (!afterS.empty())
            {
                throw ifstream::failure("");
            }
        }
    }
    catch (ifstream::failure& e)
    {
        if (!afterS.empty())
        {
            stringstream ss;
            ss << "Error reading execution order line from " << _filename << endl;
            throw WorkflowParsingException(ss.str());
        }
    }

    vector<size_t> executionOrder;

    regex idRegex("^id([0-9]+)( -> |$)");
    smatch idSmatch;
    bool hasArrow = false;

    while (regex_search(s, idSmatch, idRegex))
    {
        size_t id;
        if (!str2size_t(id, idSmatch[1].str()))
        {
            stringstream ss;
            ss << "Error parsing number (in line " << s << ")\n";
            throw WorkflowParsingException(ss.str());
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
        throw WorkflowParsingException("Error reading execution order line\n");
    }

    return executionOrder;
}
