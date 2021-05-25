package ru.nsu.fit.g19202.dmakogon.task2.calc.commands;

import ru.nsu.fit.g19202.dmakogon.task2.calc.CommandContext;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.DivisionByZeroException;

public class Divide extends Command
{
    public Divide()
    {
        super(0, 2);
    }

    @Override
    protected void executeImplementation(CommandContext context, Object[] params) throws DivisionByZeroException
    {
        double op1 = context.pop();
        double op2 = context.pop();

        if (op1 == 0)
        {
            throw new DivisionByZeroException(createExceptionMessage("Division by zero"));
        }

        context.push(op2 / op1);
    }
}
