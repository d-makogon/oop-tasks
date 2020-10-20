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

        const string& inputFile = argsParser.GetCmdOption("-i");
        const string& outputFile = argsParser.GetCmdOption("-o");

        auto workers = wfParser.ParseWorkers();
        auto execOrder = wfParser.ParseExecutionOrder();

        WorkflowExecutor we(workers, execOrder);

        if (!inputFile.empty())
        {
            we.SetCustomInputFile(inputFile);
        }
        if (!outputFile.empty())
        {
            we.SetCustomOutputFile(outputFile);
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
