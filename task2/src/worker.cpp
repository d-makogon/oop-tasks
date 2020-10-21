#include "worker.h"
#include <sstream>

using namespace std;


WorkerExecutionException::WorkerExecutionException(initializer_list<string> list)
{
    stringstream ss;
    for (const auto& item : list)
    {
        ss << item;
    }
    message = ss.str();
}

bool WorkersFactory::Register(const string& name, WorkersFactory::TCreateMethod funcCreate)
{
    auto it = s_methods.find(name);
    if (it == s_methods.end())
    {
        s_methods[name] = funcCreate;
        return true;
    }
    return false;
}

shared_ptr<Worker> WorkersFactory::Create(const string& name, const vector<string>& args)
{
    auto it = s_methods.find(name);
    if (it != s_methods.end())
    {
        return it->second(args);
    }
    return nullptr;
}
