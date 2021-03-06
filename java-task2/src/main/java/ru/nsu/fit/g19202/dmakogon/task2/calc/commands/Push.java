package ru.nsu.fit.g19202.dmakogon.task2.calc.commands;

import ru.nsu.fit.g19202.dmakogon.task2.calc.CommandContext;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.InvalidParametersException;

public class Push extends Command
{
    public Push()
    {
        super(1, 0);
    }

    @Override
    protected void executeImplementation(CommandContext context, Object[] params) throws InvalidParametersException
    {
        String str;
        try
        {
            str = (String) params[0];
        }
        catch (ClassCastException e)
        {
            throw new InvalidParametersException(createExceptionMessage("Unknown parameter type."));
        }

        try
        {
            context.push(Double.parseDouble(str));
        }
        catch (NumberFormatException e)
        {
            if (context.hasNamedParameter(str))
            {
                context.push(context.getNamedParameter(str));
            }
            else
            {
                throw new InvalidParametersException(createExceptionMessage("Unknown token '" + str + "'."));
            }
        }
    }
}
