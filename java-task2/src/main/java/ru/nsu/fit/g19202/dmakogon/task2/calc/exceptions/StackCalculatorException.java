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
        String s = getClass().getSimpleName();
        String message = getMessage();
        return (message != null) ? (s + ": " + message) : s;
    }

    @Override
    public String getMessage()
    {
        return msg;
    }
}
