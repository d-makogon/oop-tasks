package ru.nsu.fit.g19202.dmakogon.task2.calc;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.fit.g19202.dmakogon.task2.calc.commands.Command;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.InvalidCommandException;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.LineNumberReader;
import java.util.Arrays;

public class DefaultCommandsReader implements CommandsReader
{
    private static final Logger logger = LogManager.getLogger();

    private final LineNumberReader reader;
    private final CommandsFactory commandsFactory;

    public DefaultCommandsReader(InputStream inputStream)
    {
        logger.trace("Constructing new DefaultCommandsReader...");

        reader = new LineNumberReader(new InputStreamReader(inputStream));
        commandsFactory = new CommandsFactory();
    }

    @Override
    public CommandInfo nextCommand() throws IOException, InvalidCommandException
    {
        logger.trace("Trying to read next command...");

        String line = reader.readLine();

        if (line == null || 0 == line.length()) return null;

        // split values by spaces
        String[] split = line.split("\\s+");

        Command cmd;
        try
        {
            cmd = commandsFactory.create(split[0]);
        }
        catch (InvalidCommandException e)
        {
            throw new InvalidCommandException("Error at line " + reader.getLineNumber() + ": " + e.getMessage());
        }

        // remove first value (command name)
        String[] params = Arrays.copyOfRange(split, 1, split.length);

        return new CommandInfo(cmd, params);
    }

    @Override
    public void close() throws IOException
    {
        logger.trace("Closing DefaultCommandsReader.");
        reader.close();
    }
}
