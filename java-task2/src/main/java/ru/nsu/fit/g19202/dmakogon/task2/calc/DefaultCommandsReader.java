package ru.nsu.fit.g19202.dmakogon.task2.calc;

import ru.nsu.fit.g19202.dmakogon.task2.calc.commands.Command;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.InvalidCommandException;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.LineNumberReader;
import java.util.Arrays;
import java.util.logging.Logger;

public class DefaultCommandsReader implements CommandsReader
{
    private final LineNumberReader reader;
    private final CommandsFactory commandsFactory;

    private final Logger logger;

    public DefaultCommandsReader(InputStream inputStream)
    {
        logger = Logger.getLogger(getClass().getName());

        reader = new LineNumberReader(new InputStreamReader(inputStream));
        commandsFactory = new CommandsFactory();
    }

    @Override
    public CommandInfo nextCommand() throws IOException, InvalidCommandException
    {
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
            throw new InvalidCommandException("Error at line " + reader.getLineNumber() + ": " + e.toString());
        }

        // remove first value (command name)
        String[] params = Arrays.copyOfRange(split, 1, split.length);

        return new CommandInfo(cmd, params);
    }

    @Override
    public void close() throws IOException
    {
        logger.info("Closing Reader.");
        reader.close();
    }
}
