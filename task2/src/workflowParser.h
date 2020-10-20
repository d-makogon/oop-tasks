#pragma once

#include <exception>
#include <string>
#include <utility>
#include <fstream>
#include <unordered_map>
#include <memory>

#include "worker.h"

using namespace std;

class WorkflowParsingException : public exception
{
public:
    string message;

    explicit WorkflowParsingException(string message) : message(move(message)) {}
    WorkflowParsingException(initializer_list<string> list);

    [[nodiscard]] const char* what() const noexcept override { return message.c_str(); }
};

class WorkflowParser
{
private:
    ifstream _inputStream;
    string _filename;

public:
    explicit WorkflowParser(const string& filename);

    shared_ptr<unordered_map<size_t, Worker*>> ParseWorkers();

    vector<size_t> ParseExecutionOrder();
};
