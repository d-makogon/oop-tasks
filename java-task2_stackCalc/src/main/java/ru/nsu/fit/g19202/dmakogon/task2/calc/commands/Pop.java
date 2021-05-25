package ru.nsu.fit.g19202.dmakogon.task2.calc.commands;

import ru.nsu.fit.g19202.dmakogon.task2.calc.CommandContext;

public class Pop extends Command
{
    public Pop()
    {
        super(0, 1);
    }

    @Override
    protected void executeImplementation(CommandContext context, Object[] params)
    {
        context.pop();
    }
}
