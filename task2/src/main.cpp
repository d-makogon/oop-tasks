#include <iostream>
#include <regex>
#include <exception>

#include "worker.h"
#include "workers.h"
#include "workflowParser.h"
#include "workflowExecutor.h"
#include "argsParser.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "Too few arguments\n" << endl;
        return 0;
    }
    try
    {
        WorkflowParser wfParser(argv[1]);
        ArgsParser argsParser(argc, argv);

        auto workers = wfParser.ParseWorkers();
        auto execOrder = wfParser.ParseExecutionOrder();

        WorkflowExecutor we(workers, execOrder);

        if (argsParser.CmdOptionExists("-i"))
        {
            we.SetCustomInputFile(argsParser.GetCmdOption("-i"));
        }
        if (argsParser.CmdOptionExists("-o"))
        {
            we.SetCustomOutputFile(argsParser.GetCmdOption("-o"));
        }

        we.Execute();

        for (auto& it : *workers)
        {
            delete it.second;
        }
    }
    catch (exception& e)
    {
        cout << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
