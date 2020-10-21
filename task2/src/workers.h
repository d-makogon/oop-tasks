#pragma once

#include <memory>
#include <string>

#include "worker.h"

// Input - none, output - text
class ReadFileWorker : public Worker
{
public:
    explicit ReadFileWorker(std::string filename) : Worker(ReturnType::NONE, ReturnType::TEXT),
                                                    filename(std::move(filename)) {}

    static std::shared_ptr<Worker> CreateMethod(const std::vector<std::string>& args)
    {
        return std::make_shared<ReadFileWorker>(args[0]);
    }

    static std::string GetWorkerName() { return "readfile"; }
    ReadFileWorker(const size_t id, string filename) : Worker(id, ReturnType::NONE, ReturnType::TEXT),
                                                       filename(move(filename)) {}

    WorkerResult Execute(const WorkerResult& prev) override;

private:
    const std::string filename;

    [[maybe_unused]] static bool f_registered;
};

// Input - text, output - none
class WriteFileWorker : public Worker
{
public:
    explicit WriteFileWorker(std::string filename) : Worker(ReturnType::TEXT, ReturnType::NONE),
                                                     filename(move(filename)) {}

    static std::shared_ptr<Worker> CreateMethod(const std::vector<std::string>& args)
    {
        return std::make_shared<WriteFileWorker>(args[0]);
    }

    static std::string GetWorkerName() { return "writefile"; }

    WorkerResult Execute(const WorkerResult& prev) override;

private:
    const std::string filename;

    static bool f_registered;
};

// Input - text, output - text
class GrepWorker : public Worker
{
public:
    explicit GrepWorker(std::string word) : Worker(ReturnType::TEXT, ReturnType::TEXT), word(move(word)) {}

    static std::shared_ptr<Worker> CreateMethod(const std::vector<std::string>& args)
    {
        return std::make_shared<GrepWorker>(args[0]);
    }

    static std::string GetWorkerName() { return "grep"; }

    WorkerResult Execute(const WorkerResult& prev) override;

private:
    const std::string word;

    static bool f_registered;
};

// Input - text, output - text
class SortWorker : public Worker
{
public:
    explicit SortWorker() : Worker(ReturnType::TEXT, ReturnType::TEXT) {}

    static std::shared_ptr<Worker> CreateMethod(const std::vector<std::string>& args)
    {
        return std::make_shared<SortWorker>();
    }

    static std::string GetWorkerName() { return "sort"; }

    WorkerResult Execute(const WorkerResult& prev) override;

private:
    static bool f_registered;
};

// Input - text, output - text
class ReplaceWorker : public Worker
{
public:
    ReplaceWorker(std::string word1, std::string word2) : Worker(ReturnType::TEXT, ReturnType::TEXT),
                                                          word1(std::move(word1)), word2(std::move(word2)) {}

    static std::shared_ptr<Worker> CreateMethod(const std::vector<std::string>& args)
    {
        return std::make_shared<ReplaceWorker>(args[0], args[1]);
    }

    static std::string GetWorkerName() { return "replace"; }

    WorkerResult Execute(const WorkerResult& prev) override;

private:
    const std::string word1;
    const std::string word2;

    static bool f_registered;
};

// Input - text, output - text
class DumpWorker : public Worker
{
public:
    explicit DumpWorker(std::string filename) : Worker(ReturnType::TEXT, ReturnType::TEXT),
                                                filename(std::move(filename)) {}

    static std::shared_ptr<Worker> CreateMethod(const std::vector<std::string>& args)
    {
        return std::make_shared<DumpWorker>(args[0]);
    }

    static std::string GetWorkerName() { return "dump"; }

    WorkerResult Execute(const WorkerResult& prev) override;

private:
    const std::string filename;

    static bool f_registered;
};
