package ru.nsu.fit.g19202.dmakogon.threads;

public class ManagedThread extends Thread
{
    private final Object lock = new Object();
    private ThreadState desiredState = ThreadState.RUNNING;

    protected synchronized ThreadState getDesiredState()
    {
        return desiredState;
    }

    public void managedStop()
    {
        synchronized (lock)
        {
            desiredState = ThreadState.STOP;
            lock.notifyAll();
        }
    }

    public void managedSuspend()
    {
        synchronized (lock)
        {
            if (desiredState != ThreadState.STOP)
            {
                desiredState = ThreadState.SLEEP;
            }
        }
    }

    public void managedResume()
    {
        synchronized (lock)
        {
            if (desiredState == ThreadState.SLEEP)
            {
                desiredState = ThreadState.RUNNING;
                lock.notifyAll();
            }
        }
    }

    protected boolean keepRunning()
    {
        synchronized (lock)
        {
            if (isInterrupted())
            {
                // System.err.println(Thread.currentThread().getName() + " was interrupted by flag");
                return false;
            }
            if (desiredState == ThreadState.RUNNING)
            {
                return true;
            }
            else
            {
                while (true)
                {
                    if (isInterrupted())
                    {
                        return false;
                    }
                    else if (desiredState == ThreadState.STOP)
                    {
                        // System.err.println(Thread.currentThread().getName() + " is stopped");
                        return false;
                    }
                    else if (desiredState == ThreadState.SLEEP)
                    {
                        try
                        {
                            // System.err.println(Thread.currentThread().getName() + " fell asleep");
                            lock.wait();
                        }
                        catch (InterruptedException e)
                        {
                            // System.err.println(Thread.currentThread().getName() + " is interrupted");
                            return false;
                        }
                    }
                    else if (desiredState == ThreadState.RUNNING)
                    {
                        // System.err.println(Thread.currentThread().getName() + " is resumed");
                        return true;
                    }
                }
            }
        }
    }
}
