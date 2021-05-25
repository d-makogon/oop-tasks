package ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions;

public class DivisionByZeroException extends InvalidParametersException
{
    public DivisionByZeroException(String msg)
    {
        super(msg);
    }
}
