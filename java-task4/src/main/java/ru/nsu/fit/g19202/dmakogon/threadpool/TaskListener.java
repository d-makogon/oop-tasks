package ru.nsu.fit.g19202.dmakogon.threadpool;

public interface TaskListener
{
    void taskStarted(Task task);

    void taskInterrupted(Task task);

    void taskFinished(Task task);
}
