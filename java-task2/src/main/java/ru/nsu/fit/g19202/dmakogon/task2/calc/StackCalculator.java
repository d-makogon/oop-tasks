package ru.nsu.fit.g19202.dmakogon.task2.calc;

import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.StackCalculatorException;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintStream;
import java.util.logging.Logger;

public class StackCalculator
{
    private final Logger logger;
    private final ICommandContext context;
    private final CommandsReader commandsReader;

    public StackCalculator(InputStream inputStream, OutputStream outputStream)
    {
        logger = Logger.getLogger(StackCalculator.class.getName());
        logger.info("Constructing new StackCalculator.");

        context = new CommandExecutionContext(outputStream);
        commandsReader = new DefaultCommandsReader(inputStream);
    }

    public void execute() throws IOException
    {
        logger.info("Starting execution.");

        CommandInfo cmdInfo;

        while (true)
        {
            try
            {
                cmdInfo = commandsReader.nextCommand();
                if (cmdInfo == null) break;
                cmdInfo.getCommand().execute(context, cmdInfo.getParams());
            }
            catch (StackCalculatorException e)
            {
                logger.warning(e.getMessage());
            }
        }
        commandsReader.close();
        context.close();
        logger.info("Finished execution.");
    }
}
