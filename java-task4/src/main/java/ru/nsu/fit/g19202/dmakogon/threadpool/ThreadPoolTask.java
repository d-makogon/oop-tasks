package ru.nsu.fit.g19202.dmakogon.threadpool;

class ThreadPoolTask
{
    private final Task task;
    private final TaskListener taskListener;

    public ThreadPoolTask(Task task, TaskListener taskListener)
    {
        this.task = task;
        this.taskListener = taskListener;
    }

    public void prepare()
    {
        taskListener.taskStarted(task);
    }

    public void go() throws InterruptedException
    {
        task.performWork();
    }

    public void finish()
    {
        taskListener.taskFinished(task);
    }

    public String getName()
    {
        return task.getName();
    }

    public void interrupted()
    {
        taskListener.taskInterrupted(task);
    }
}
