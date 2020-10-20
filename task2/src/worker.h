#pragma once

#include <utility>
#include <vector>
#include <string>
#include <exception>

using namespace std;

class WorkerExecutionException : public exception
{
public:
    string message;

    explicit WorkerExecutionException(string message) : message(move(message)) {}

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
        vector<string> text;

    public:
        WorkerResult() : text(vector<string>()) {}

        explicit WorkerResult(vector<string>& text) : text(text) {}

        [[nodiscard]] vector<string> GetText() const { return text; }
    };

    ReturnType GetInputType() { return inputType; }

    ReturnType GetOutputType() { return outputType; }

    virtual WorkerResult Execute(const WorkerResult& prev) = 0;

    virtual ~Worker() = default;

protected:
    size_t _id;
    ReturnType inputType;
    ReturnType outputType;

    Worker(const size_t id, ReturnType inputType, ReturnType outputType) : _id(id),
                                                                           inputType(inputType),
                                                                           outputType(outputType) {}
};
