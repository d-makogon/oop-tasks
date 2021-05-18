package ru.nsu.fit.g19202.dmakogon.threads;

public enum ThreadState
{
    RUNNING(0), SLEEP(1), STOP(2);

    private final int value;

    ThreadState(int value)
    {
        this.value = value;
    }

    public static ThreadState fromValue(int value)
    {
        if (value < 0 || value > values().length - 1)
        {
            throw new IllegalArgumentException(value + " is out of range for " + ThreadState.class.getName());
        }
        for (ThreadState threadState : ThreadState.values())
        {
            if (threadState.getValue() == value)
            {
                return threadState;
            }
        }
        throw new IllegalArgumentException("Wrong value: " + value);
    }

    public int getValue()
    {
        return value;
    }
}
