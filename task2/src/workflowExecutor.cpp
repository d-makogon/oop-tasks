#include "workflowExecutor.h"

#include <utility>
#include <sstream>

void WorkflowExecutor::Execute()
{
    Worker::WorkerResult wr;
    size_t cmdsToExecCount = _execOrder.size();

    Worker* rfWorker;
    Worker* wfWorker;
    Worker* prevWorker;
    Worker* currWorker;

    if (_useCustomInputFile)
    {
        rfWorker = new ReadFileWorker(0, _inputFileName);
    }

    if (_useCustomOutputFile)
    {
        wfWorker = new WriteFileWorker(0, _outputFileName);
    }

    for (size_t i = 0; i < cmdsToExecCount; i++)
    {
        prevWorker = currWorker;
        currWorker = (*_workers)[_execOrder[i]];
        if ((i == 0) && _useCustomInputFile)
        {
            wr = rfWorker->Execute(wr);
            prevWorker = rfWorker;
        }

        if ((i > 0 || _useCustomInputFile) && prevWorker->GetOutputType() != currWorker->GetInputType())
        {
            throw WorkerExecutionException("Conflicting output and input types of blocks\n");
        }

        wr = currWorker->Execute(wr);

        if ((i == cmdsToExecCount - 1) && _useCustomOutputFile)
        {
            if (currWorker->GetOutputType() != wfWorker->GetInputType())
            {
                throw WorkerExecutionException("Conflicting output and input types of blocks\n");
            }
            wr = wfWorker->Execute(wr);
        }
    }
}

void WorkflowExecutor::SetCustomInputFile(string filename)
{
    _inputFileName = std::move(filename);
    _useCustomInputFile = true;
}

void WorkflowExecutor::SetCustomOutputFile(string filename)
{
    _outputFileName = std::move(filename);
    _useCustomOutputFile = true;
}

WorkflowExecutionException::WorkflowExecutionException(initializer_list<string> list)
{
    stringstream ss;
    for (const auto& item : list)
    {
        ss << item;
    }
    message = ss.str();
}
