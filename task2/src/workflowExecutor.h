#pragma once

#include <unordered_map>
#include <memory>
#include <utility>
#include <fstream>
#include <exception>

#include "worker.h"
#include "workers.h"

class WorkflowExecutionException : public std::exception
{
public:
    std::string message;

    explicit WorkflowExecutionException(std::string message) : message(std::move(message)) {}
    WorkflowExecutionException(std::initializer_list<std::string> list);

    [[nodiscard]] const char* what() const noexcept override { return message.c_str(); }
};


class WorkflowExecutor
{
private:
    using TWorkersMap = std::unordered_map<size_t, std::shared_ptr<Worker>>;

    std::shared_ptr<TWorkersMap> _workers;
    std::vector<size_t> _execOrder;

    std::string _inputFileName;
    std::string _outputFileName;

    bool _useCustomInputFile;
    bool _useCustomOutputFile;

public:
    WorkflowExecutor(std::shared_ptr<TWorkersMap> workers,
                     std::vector<size_t> execOrder) :
            _workers(std::move(workers)),
            _execOrder(std::move(execOrder)),
            _useCustomInputFile(false),
            _useCustomOutputFile(false) {}

    std::shared_ptr<TWorkersMap> GetWorkers() { return _workers; }

    std::vector<size_t> GetExecOrder() { return _execOrder; }

    void SetCustomInputFile(std::string filename);

    void SetCustomOutputFile(std::string filename);

    void Execute();
};
