#pragma once

#include <utility>
#include <vector>
#include <string>
#include <exception>
#include <memory>
#include <map>

class WorkerExecutionException : public std::exception
{
public:
    std::string message;

    explicit WorkerExecutionException(std::string message) : message(std::move(message)) {}

    WorkerExecutionException(std::initializer_list<std::string> list);

    [[nodiscard]] const char* what() const noexcept override { return message.c_str(); }
};

class Worker
{
public:
    enum class ReturnType
    {
        NONE, TEXT
    };

    class WorkerResult
    {
    private:
        std::vector<std::string> text;

    public:
        WorkerResult() : text(std::vector<std::string>()) {}

        explicit WorkerResult(std::vector<std::string>& text) : text(text) {}

        [[nodiscard]] std::vector<std::string> GetText() const { return text; }
    };

    ReturnType GetInputType() { return inputType; }

    ReturnType GetOutputType() { return outputType; }

    virtual WorkerResult Execute(const WorkerResult& prev) = 0;

    virtual ~Worker() = default;

protected:
    ReturnType inputType;
    ReturnType outputType;

    Worker(ReturnType inputType, ReturnType outputType) : inputType(inputType),
                                                          outputType(outputType) {}
};

class WorkersFactory
{
public:
    using TCreateMethod = std::shared_ptr<Worker> (*)(const std::vector<std::string>& args);

    WorkersFactory() = delete;

    static bool Register(const std::string& name, TCreateMethod funcCreate);

    static std::shared_ptr<Worker> Create(const std::string& name, const std::vector<std::string>& args);

private:
    static std::map<std::string, TCreateMethod> s_methods;
};
