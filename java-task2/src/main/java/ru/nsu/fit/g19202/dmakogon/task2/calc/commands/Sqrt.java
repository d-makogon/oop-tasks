package ru.nsu.fit.g19202.dmakogon.task2.calc.commands;

import ru.nsu.fit.g19202.dmakogon.task2.calc.CommandContext;

public class Sqrt extends Command
{
    public Sqrt()
    {
        super(0, 1);
    }

    @Override
    protected void executeImplementation(CommandContext context, Object[] params)
    {
        context.push(Math.sqrt(context.pop()));
    }
}
