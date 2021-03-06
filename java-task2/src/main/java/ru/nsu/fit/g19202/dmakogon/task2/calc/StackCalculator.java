package ru.nsu.fit.g19202.dmakogon.task2.calc;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.StackCalculatorException;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class StackCalculator
{
    private final Logger logger = LogManager.getLogger();
    private final CommandContext context;
    private final CommandsReader commandsReader;

    public StackCalculator(InputStream inputStream, OutputStream outputStream)
    {
        logger.trace("Constructing new StackCalculator...");

        context = new CommandExecutionContext(outputStream);
        commandsReader = new DefaultCommandsReader(inputStream);
    }

    public void execute() throws IOException
    {
        logger.trace("Starting execution.");

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
                System.err.println(e.getMessage());
                logger.error(e.toString());
            }
        }
        commandsReader.close();
        context.close();
        logger.info("Finished execution.");
    }
}
