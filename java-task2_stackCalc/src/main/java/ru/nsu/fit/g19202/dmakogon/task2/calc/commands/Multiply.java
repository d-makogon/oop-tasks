package ru.nsu.fit.g19202.dmakogon.task2.calc.commands;

import ru.nsu.fit.g19202.dmakogon.task2.calc.CommandContext;

public class Multiply extends Command
{
    public Multiply()
    {
        super(0, 2);
    }

    @Override
    protected void executeImplementation(CommandContext context, Object[] params)
    {
        double op1 = context.pop();
        double op2 = context.pop();

        context.push(op1 * op2);
    }
}
