#pragma once

#include <unordered_map>
#include <memory>
#include <utility>
#include <fstream>
#include <exception>

#include "worker.h"
#include "workers.h"

class WorkflowExecutionException : public exception
{
public:
    string message;

    explicit WorkflowExecutionException(string message) : message(move(message)) {}

    [[nodiscard]] const char* what() const noexcept override { return message.c_str(); }
};


class WorkflowExecutor
{
private:
    shared_ptr<unordered_map<size_t, Worker*>> _workers;
    vector<size_t> _execOrder;

    string _inputFileName;
    string _outputFileName;

    bool _useCustomInputFile;
    bool _useCustomOutputFile;

public:
    WorkflowExecutor(shared_ptr<unordered_map<size_t, Worker*>> workers,
                     const vector<size_t>& execOrder) :
            _workers(std::move(workers)),
            _execOrder(execOrder),
            _useCustomInputFile(false),
            _useCustomOutputFile(false) {}

    shared_ptr<unordered_map<size_t, Worker*>> GetWorkers() { return _workers; }

    vector<size_t> GetExecOrder() { return _execOrder; }

    void SetCustomInputFile(string filename);

    void SetCustomOutputFile(string filename);

    void Execute();
};
