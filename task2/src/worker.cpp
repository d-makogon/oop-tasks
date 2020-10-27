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
