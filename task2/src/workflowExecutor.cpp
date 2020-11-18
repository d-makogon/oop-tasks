#include <workflowExecutor.h>

#include <utility>
#include <sstream>
#include <memory>

using namespace std;

void WorkflowExecutor::Execute()
{
    Worker::WorkerResult wr;
    size_t cmdsToExecCount = _execOrder.size();

    shared_ptr<Worker> rfWorker;
    shared_ptr<Worker> wfWorker;
    shared_ptr<Worker> prevWorker;
    shared_ptr<Worker> currWorker;

    if (_useCustomInputFile)
    {
        rfWorker = make_shared<ReadFileWorker>(_inputFileName);
    }

    if (_useCustomOutputFile)
    {
        wfWorker = make_shared<WriteFileWorker>(_outputFileName);
    }

    for (size_t i = 0; i < cmdsToExecCount; i++)
    {
        prevWorker = currWorker;
        currWorker = (*_workers)[_execOrder[i]];

        if (currWorker == nullptr)
        {
            throw WorkflowException({"No worker found for id", to_string(_execOrder[i]), "\n"});
        }

        if ((i == 0) && _useCustomInputFile)
        {
            wr = rfWorker->Execute(wr);
            prevWorker = rfWorker;
        }

        if ((i > 0 || _useCustomInputFile) && prevWorker->GetOutputType() != currWorker->GetInputType())
        {
            throw WorkflowException("Conflicting output and input types of blocks\n");
        }

        wr = currWorker->Execute(wr);

        if ((i == cmdsToExecCount - 1) && _useCustomOutputFile)
        {
            if (currWorker->GetOutputType() != wfWorker->GetInputType())
            {
                throw WorkflowException("Conflicting output and input types of blocks\n");
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
