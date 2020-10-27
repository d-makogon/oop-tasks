#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "worker.h"

// Input - none, output - text
class ReadFileWorker : public Worker
{
public:
    explicit ReadFileWorker(std::string filename) : Worker(ReturnType::NONE, ReturnType::TEXT),
                                                    filename(std::move(filename)) {}

    WorkerResult Execute(const WorkerResult& prev) override;

private:
    const std::string filename;
};

// Input - text, output - none
class WriteFileWorker : public Worker
{
public:
    explicit WriteFileWorker(std::string filename) : Worker(ReturnType::TEXT, ReturnType::NONE),
                                                     filename(move(filename)) {}

    WorkerResult Execute(const WorkerResult& prev) override;

private:
    const std::string filename;
};

// Input - text, output - text
class GrepWorker : public Worker
{
public:
    explicit GrepWorker(std::string word) : Worker(ReturnType::TEXT, ReturnType::TEXT), word(move(word)) {}

    WorkerResult Execute(const WorkerResult& prev) override;

private:
    const std::string word;
};

// Input - text, output - text
class SortWorker : public Worker
{
public:
    explicit SortWorker() : Worker(ReturnType::TEXT, ReturnType::TEXT) {}

    WorkerResult Execute(const WorkerResult& prev) override;
};

// Input - text, output - text
class ReplaceWorker : public Worker
{
public:
    ReplaceWorker(std::string word1, std::string word2) : Worker(ReturnType::TEXT, ReturnType::TEXT),
                                                          word1(std::move(word1)), word2(std::move(word2)) {}

    WorkerResult Execute(const WorkerResult& prev) override;

private:
    const std::string word1;
    const std::string word2;
};

// Input - text, output - text
class DumpWorker : public Worker
{
public:
    explicit DumpWorker(std::string filename) : Worker(ReturnType::TEXT, ReturnType::TEXT),
                                                filename(std::move(filename)) {}

    WorkerResult Execute(const WorkerResult& prev) override;

private:
    const std::string filename;
};
