package ru.nsu.fit.g19202.dmakogon.task2.calc.commands;

import ru.nsu.fit.g19202.dmakogon.task2.calc.ICommandContext;

public class Subtract extends Command
{
    public Subtract()
    {
        super(0, 2);
    }

    @Override
    protected void executeImplementation(ICommandContext context, Object[] params)
    {
        double op1 = context.pop();
        double op2 = context.pop();

        context.push(op2 - op1);
    }
}
