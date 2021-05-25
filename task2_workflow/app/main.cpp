#include <iostream>
#include <exception>

#include <worker.h>
#include <workers.h>
#include <workersFactory.h>
#include <workflowParser.h>
#include <workflowExecutor.h>
#include <argsParser.h>

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
        Factory<Worker> factory;

        // @formatter:off
        factory.Register<ReadFileWorker>  ("readfile",  Factory<ReadFileWorker>::CreateFunc);
        factory.Register<WriteFileWorker> ("writefile", Factory<WriteFileWorker>::CreateFunc);
        factory.Register<GrepWorker>      ("grep",      Factory<GrepWorker>::CreateFunc);
        factory.Register<SortWorker>      ("sort",      Factory<SortWorker>::CreateFunc);
        factory.Register<ReplaceWorker>   ("replace",   Factory<ReplaceWorker>::CreateFunc);
        factory.Register<DumpWorker>      ("dump",      Factory<DumpWorker>::CreateFunc);
        // @formatter:on

        WorkflowParser wfParser(argv[1], factory);
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
