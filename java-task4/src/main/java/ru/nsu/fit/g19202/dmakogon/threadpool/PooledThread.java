package ru.nsu.fit.g19202.dmakogon.threadpool;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.List;
import java.util.concurrent.atomic.AtomicBoolean;

public class PooledThread extends Thread
{
    private final List<ThreadPoolTask> taskQueue;
    private final Logger logger = LogManager.getLogger();
    AtomicBoolean shutdownRequired = new AtomicBoolean(false);

    public PooledThread(String name, List<ThreadPoolTask> taskQueue)
    {
        super(name);
        this.taskQueue = taskQueue;
    }

    private void performTask(ThreadPoolTask task)
    {
        task.prepare();
        try
        {
            task.go();
        }
        catch (InterruptedException e)
        {
            task.interrupted();
            shutdownRequired.set(true);
            return;
        }
        task.finish();
    }

    @Override
    public void run()
    {
        ThreadPoolTask taskToExecute;
        while (true)
        {
            synchronized (taskQueue)
            {
                if (taskQueue.isEmpty())
                {
                    try
                    {
                        taskQueue.wait();
                    }
                    catch (InterruptedException e)
                    {
                        logger.warn("Thread " + getName() + " was interrupted");
                        break;
                    }
                    continue;
                }
                else
                {
                    taskToExecute = taskQueue.remove(0);
                }
            }
            logger.info("Thread " + getName() + " got job " + taskToExecute.getName());
            performTask(taskToExecute);
            if (shutdownRequired.get())
            {
                break;
            }
        }
    }
}
