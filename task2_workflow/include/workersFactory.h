#pragma once

#include <map>
#include <vector>
#include <memory>

#include "worker.h"
#include "workers.h"

template<typename T>
class Factory
{
public:
    typedef std::shared_ptr<T> (* TCreateFunc)(const std::vector<std::string>& args);

    template<typename TDerived>
    void Register(const std::string& name, TCreateFunc createFunc)
    {
        static_assert(std::is_base_of<T, TDerived>::value,
                      "Factory::Register doesn't accept this type because it doesn't derive from base class");

        _createFuncs[name] = createFunc;
    }

    std::shared_ptr<T> Create(std::string name, const std::vector<std::string>& args)
    {
        auto it = _createFuncs.find(name);
        if (it != _createFuncs.end())
        {
            return it->second(args);
        }
        return nullptr;
    }

private:
    std::map<std::string, TCreateFunc> _createFuncs;
};

template<>
class Factory<ReadFileWorker>
{
public:
    static std::shared_ptr<Worker> CreateFunc(const std::vector<std::string>& args)
    {
        if (args.size() != 1)
        {
            throw std::invalid_argument("Invalid number of arguments for ReadFileWorker.");
        }
        return std::make_shared<ReadFileWorker>(args[0]);
    }
};

template<>
class Factory<WriteFileWorker>
{
public:
    static std::shared_ptr<Worker> CreateFunc(const std::vector<std::string>& args)
    {
        if (args.size() != 1)
        {
            throw std::invalid_argument("Invalid number of arguments for WriteFileWorker.");
        }
        return std::make_shared<WriteFileWorker>(args[0]);
    }
};

template<>
class Factory<GrepWorker>
{
public:
    static std::shared_ptr<Worker> CreateFunc(const std::vector<std::string>& args)
    {
        if (args.size() != 1)
        {
            throw std::invalid_argument("Invalid number of arguments for GrepWorker.");
        }
        return std::make_shared<GrepWorker>(args[0]);
    }
};

template<>
class Factory<SortWorker>
{
public:
    static std::shared_ptr<Worker> CreateFunc(const std::vector<std::string>& args)
    {
        if (!args.empty())
        {
            throw std::invalid_argument("Invalid number of arguments for SortWorker.");
        }
        return std::make_shared<SortWorker>();
    }
};

template<>
class Factory<ReplaceWorker>
{
public:
    static std::shared_ptr<Worker> CreateFunc(const std::vector<std::string>& args)
    {
        if (args.size() != 2)
        {
            throw std::invalid_argument("Invalid number of arguments for ReplaceWorker.");
        }
        return std::make_shared<ReplaceWorker>(args[0], args[1]);
    }
};

template<>
class Factory<DumpWorker>
{
public:
    static std::shared_ptr<Worker> CreateFunc(const std::vector<std::string>& args)
    {
        if (args.size() != 1)
        {
            throw std::invalid_argument("Invalid number of arguments for DumpWorker.");
        }
        return std::make_shared<DumpWorker>(args[0]);
    }
};