#pragma once

#include <exception>
#include <string>
#include <utility>
#include <fstream>
#include <unordered_map>
#include <memory>

#include "worker.h"
#include "workersFactory.h"

class WorkflowParser
{
private:
    std::ifstream _inputStream;
    std::string _filename;
    Factory<Worker> _factory;

public:
    using TWorkersMap = std::unordered_map<size_t, std::shared_ptr<Worker>>;

    WorkflowParser(std::string filename, Factory<Worker> factory);

    std::shared_ptr<TWorkersMap> ParseWorkers();

    std::vector<size_t> ParseExecutionOrder();
};
