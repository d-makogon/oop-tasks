package ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions;

public class StackCalculatorException extends Exception
{
    private final String msg;

    public StackCalculatorException(String msg)
    {
        this.msg = msg;
    }

    @Override
    public String toString()
    {
        return getClass().getSimpleName() + ": " + msg;
    }

    @Override
    public String getMessage()
    {
        return toString();
    }
}
