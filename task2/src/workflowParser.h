#pragma once

#include <exception>
#include <string>
#include <utility>
#include <fstream>
#include <unordered_map>
#include <memory>

#include "worker.h"

class WorkflowParsingException : public std::exception
{
public:
    std::string message;

    explicit WorkflowParsingException(std::string message) : message(std::move(message)) {}
    WorkflowParsingException(std::initializer_list<std::string> list);

    [[nodiscard]] const char* what() const noexcept override { return message.c_str(); }
};

class WorkflowParser
{
private:
    std::ifstream _inputStream;
    std::string _filename;

public:
    using TWorkersMap = std::unordered_map<size_t, std::shared_ptr<Worker>>;

    explicit WorkflowParser(const std::string& filename);

    std::shared_ptr<TWorkersMap> ParseWorkers();

    std::vector<size_t> ParseExecutionOrder();
};
