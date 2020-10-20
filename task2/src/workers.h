#pragma once

#include "worker.h"

// Input - none, output - text
class ReadFileWorker : public Worker
{
public:
    ReadFileWorker(const size_t id, string filename) : Worker(id, ReturnType::NONE, ReturnType::TEXT),
                                                       filename(move(filename)) {}

    WorkerResult Execute(const WorkerResult& prev) override;

private:
    const string filename;
};

// Input - text, output - none
class WriteFileWorker : public Worker
{
public:
    WriteFileWorker(const size_t id, string filename) : Worker(id, ReturnType::TEXT, ReturnType::NONE),
                                                        filename(move(filename)) {}

    WorkerResult Execute(const WorkerResult& prev) override;

private:
    const string filename;
};

// Input - text, output - text
class GrepWorker : public Worker
{
public:
    GrepWorker(const size_t id, string word) : Worker(id, ReturnType::TEXT, ReturnType::TEXT), word(move(word)) {}

    WorkerResult Execute(const WorkerResult& prev) override;

private:
    const string word;
};

// Input - text, output - text
class SortWorker : public Worker
{
public:
    explicit SortWorker(const size_t id) : Worker(id, ReturnType::TEXT, ReturnType::TEXT) {}

    WorkerResult Execute(const WorkerResult& prev) override;
};

// Input - text, output - text
class ReplaceWorker : public Worker
{
public:
    ReplaceWorker(const size_t id, string word1, string word2) : Worker(id, ReturnType::TEXT, ReturnType::TEXT),
                                                                 word1(move(word1)), word2(move(word2)) {}

    WorkerResult Execute(const WorkerResult& prev) override;

private:
    const string word1;
    const string word2;
};

// Input - text, output - text
class DumpWorker : public Worker
{
public:
    DumpWorker(const size_t id, string filename) : Worker(id, ReturnType::TEXT, ReturnType::TEXT),
                                                   filename(std::move(filename)) {}

    WorkerResult Execute(const WorkerResult& prev) override;

private:
    const string filename;
};
