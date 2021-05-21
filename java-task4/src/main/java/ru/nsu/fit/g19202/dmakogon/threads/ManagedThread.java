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
                        return false;
                    }
                    else if (desiredState == ThreadState.SLEEP)
                    {
                        try
                        {
                            lock.wait();
                        }
                        catch (InterruptedException e)
                        {
                            return false;
                        }
                    }
                    else if (desiredState == ThreadState.RUNNING)
                    {
                        return true;
                    }
                }
            }
        }
    }
}
