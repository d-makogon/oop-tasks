package ru.nsu.fit.g19202.dmakogon.threadpool;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class ThreadPool implements TaskListener
{
    private static final int THREADS_COUNT = 20;
    private final Logger logger = LogManager.getLogger();
    private final List<ThreadPoolTask> taskQueue = new ArrayList<>();
    private final Set<PooledThread> availableThreads = new HashSet<>();

    public ThreadPool()
    {
        for (int i = 0; i < THREADS_COUNT; i++)
        {
            availableThreads.add(new PooledThread("Performer_" + i, taskQueue));
        }
        for (PooledThread t : availableThreads)
        {
            t.start();
        }
    }

    @Override
    public void taskStarted(Task task)
    {
        logger.info("Started " + task.getName() + " task");
    }

    @Override
    public void taskInterrupted(Task task)
    {
        logger.info("Interrupted " + task.getName() + " task");
    }

    @Override
    public void taskFinished(Task task)
    {
        logger.info("Finished " + task.getName() + " task");
    }

    public void addTask(Task task)
    {
        addTask(task, this);
    }

    public void addTask(Task task, TaskListener taskListener)
    {
        synchronized (taskQueue)
        {
            taskQueue.add(new ThreadPoolTask(task, taskListener));
            taskQueue.notify();
        }
    }

    public void shutdown()
    {
        logger.info("Shutting down pool by interrupting threads...");
        for (PooledThread t : availableThreads)
        {
            t.interrupt();
        }
        for (PooledThread t : availableThreads)
        {
            try
            {
                t.join();
            }
            catch (InterruptedException e)
            {
                logger.warn("Interrupted for join in " + t.getName() + " thread");
            }
        }
        logger.info("Shutdown complete");
    }
}
