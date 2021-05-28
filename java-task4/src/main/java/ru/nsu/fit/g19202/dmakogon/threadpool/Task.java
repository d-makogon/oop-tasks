package ru.nsu.fit.g19202.dmakogon.threadpool;

public interface Task
{
    String getName();

    void performWork() throws InterruptedException;
}
