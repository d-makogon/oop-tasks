package ru.nsu.fit.g19202.dmakogon.task2.calc.commands;

import ru.nsu.fit.g19202.dmakogon.task2.calc.ICommandContext;

public class Pop extends Command
{
    protected Pop()
    {
        super(0, 1);
    }

    @Override
    protected void executeImplementation(ICommandContext context, Object[] params)
    {
        context.pop();
    }
}
