package ru.nsu.fit.g19202.dmakogon.task2.calc.commands;

import ru.nsu.fit.g19202.dmakogon.task2.calc.CommandContext;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.WriteException;

import java.io.IOException;
import java.io.Writer;

public class Print extends Command
{
    public Print()
    {
        super(0, 1);
    }

    @Override
    protected void executeImplementation(CommandContext context, Object[] params) throws WriteException
    {
        try
        {
            Writer writer = context.getWriter();
            writer.append(Double.toString(context.peek())).append(System.lineSeparator());
            writer.flush();
        }
        catch (IOException e)
        {
            throw new WriteException(createExceptionMessage(e.getMessage()));
        }
    }
}
