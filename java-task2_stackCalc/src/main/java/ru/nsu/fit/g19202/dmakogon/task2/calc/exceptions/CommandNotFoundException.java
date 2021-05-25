package ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions;

public class CommandNotFoundException extends StackCalculatorException
{
    public CommandNotFoundException(String msg)
    {
        super(msg);
    }
}
