#include <iostream>
#include <regex>
#include <exception>

#include "worker.h"
#include "workers.h"
#include "workflowParser.h"
#include "workflowExecutor.h"
#include "argsParser.h"

using namespace std;

map<string, WorkersFactory::TCreateMethod> WorkersFactory::s_methods;

bool ReadFileWorker::f_registered = WorkersFactory::Register(ReadFileWorker::GetWorkerName(),
                                                             ReadFileWorker::CreateMethod);

bool WriteFileWorker::f_registered = WorkersFactory::Register(WriteFileWorker::GetWorkerName(),
                                                              WriteFileWorker::CreateMethod);

bool GrepWorker::f_registered = WorkersFactory::Register(GrepWorker::GetWorkerName(), GrepWorker::CreateMethod);

bool SortWorker::f_registered = WorkersFactory::Register(SortWorker::GetWorkerName(), SortWorker::CreateMethod);

bool ReplaceWorker::f_registered = WorkersFactory::Register(ReplaceWorker::GetWorkerName(),
                                                            ReplaceWorker::CreateMethod);

bool DumpWorker::f_registered = WorkersFactory::Register(DumpWorker::GetWorkerName(), DumpWorker::CreateMethod);

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
    }
    catch (exception& e)
    {
        cout << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
