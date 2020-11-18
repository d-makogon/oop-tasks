#include <workers.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>

using namespace std;

Worker::WorkerResult ReadFileWorker::Execute(const Worker::WorkerResult& prev)
{
    ifstream file;
    file.exceptions(ifstream::failbit | ifstream::badbit);

    vector<string> lines;
    string line;
    try
    {
        file.open(filename);
        while (!file.eof() && getline(file, line))
        {
            lines.push_back(line);
        }
        file.close();
    }
    catch (ifstream::failure& e)
    {
        if (!file.eof())
        {
            throw WorkflowException({"Error opening/reading/closing file ", filename, "\n"});
        }
    }
    return WorkerResult(lines);
}

Worker::WorkerResult WriteFileWorker::Execute(const Worker::WorkerResult& prev)
{
    ofstream file;
    file.exceptions(ofstream::failbit | ofstream::badbit);

    try
    {
        file.open(filename);
        for (const auto& it : prev.GetText())
        {
            file << it << endl;
        }
        file.close();
    }
    catch (ofstream::failure& e)
    {
        throw WorkflowException({"Error opening/writing to/closing file ", filename});
    }
    return Worker::WorkerResult();
}

Worker::WorkerResult GrepWorker::Execute(const Worker::WorkerResult& prev)
{
    vector<string> result;

    for (const auto& s : prev.GetText())
    {
        if (s.find(word) != string::npos)
        {
            result.push_back(s);
        }
    }

    return Worker::WorkerResult(result);
}

Worker::WorkerResult SortWorker::Execute(const Worker::WorkerResult& prev)
{
    vector<string> v = prev.GetText();
    sort(v.begin(), v.end());
    return Worker::WorkerResult(v);
}

void replaceSubstring(string& s, const string& subs1, const string& subs2)
{
    size_t index = 0;
    while (true)
    {
        index = s.find(subs1, index);
        if (index == string::npos) return;

        s.replace(index, subs2.length(), subs2);

        index += subs2.length();
    }
}

Worker::WorkerResult ReplaceWorker::Execute(const Worker::WorkerResult& prev)
{
    auto result = vector<string>();

    for (auto& it : prev.GetText())
    {
        replaceSubstring(it, word1, word2);
        result.push_back(it);
    }

    return Worker::WorkerResult(result);
}

Worker::WorkerResult DumpWorker::Execute(const Worker::WorkerResult& prev)
{
    ofstream file;
    file.exceptions(ofstream::failbit | ofstream::badbit);

    try
    {
        file.open(filename);
        for (const auto& it : prev.GetText())
        {
            file << it << endl;
        }
        file.close();
    }
    catch (ofstream::failure& e)
    {
        throw WorkflowException({"Error opening/writing to/closing file ", filename});
    }
    return prev;
}

#pragma clang diagnostic pop