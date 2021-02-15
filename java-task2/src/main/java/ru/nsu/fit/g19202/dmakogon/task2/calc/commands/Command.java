package ru.nsu.fit.g19202.dmakogon.task2.calc.commands;

import ru.nsu.fit.g19202.dmakogon.task2.calc.ICommandContext;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.EmptyStackException;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.InvalidParametersException;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.StackCalculatorException;

public abstract class Command
{
    protected int paramsCount;
    protected int stackArgsCount;

    protected Command(int paramsCount, int stackArgsCount)
    {
        this.paramsCount = paramsCount;
        this.stackArgsCount = stackArgsCount;
    }

    protected String createExceptionMessage(String msg)
    {
        return getClass().getSimpleName() + ": " + msg;
    }

    protected abstract void executeImplementation(ICommandContext context, Object[] params) throws StackCalculatorException;

    public void execute(ICommandContext context, Object[] params) throws StackCalculatorException
    {
        if (params.length > paramsCount)
        {
            throw new InvalidParametersException(createExceptionMessage("Unexpected parameter: " + params[paramsCount]));
        }
        else if (params.length < paramsCount)
        {
            throw new InvalidParametersException(createExceptionMessage("Too few parameters. Expected " + paramsCount + ", given " + params.length + "."));
        }

        int stackLength = context.getStackSize();

        if (stackLength < stackArgsCount)
        {
            throw new EmptyStackException(createExceptionMessage("Too few arguments on stack."));
        }

        executeImplementation(context, params);
    }
}
