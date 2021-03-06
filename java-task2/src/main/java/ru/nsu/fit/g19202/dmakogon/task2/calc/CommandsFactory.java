package ru.nsu.fit.g19202.dmakogon.task2.calc;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.fit.g19202.dmakogon.task2.calc.commands.Command;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.InvalidCommandException;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.MissingResourceException;
import java.util.PropertyResourceBundle;

public class CommandsFactory
{
    private final static Logger logger = LogManager.getLogger();

    private static final String COMMANDS_PROP = "/commands.properties";
    private final HashMap<String, String> commandsClassesNames;

    public CommandsFactory()
    {
        logger.trace("Constructing new CommandsFactory...");

        commandsClassesNames = new HashMap<>();

        InputStream propIn = getClass().getResourceAsStream(COMMANDS_PROP);

        if (propIn == null)
        {
            String msg = "Unable to find '" + COMMANDS_PROP + "' bundle.";
            logger.fatal(msg);
            throw new MissingResourceException(msg, COMMANDS_PROP, "");
        }

        PropertyResourceBundle bundle;
        try
        {
            bundle = new PropertyResourceBundle(new InputStreamReader(propIn));
        }
        catch (IOException e)
        {
            String msg = "Unable to open '" + COMMANDS_PROP + "' bundle.";
            logger.fatal(msg);
            throw new MissingResourceException(msg, COMMANDS_PROP, "");
        }

        logger.info("Reading keys from " + COMMANDS_PROP + ".");
        var propKeys = bundle.getKeys();

        int count = 0;
        while (propKeys.hasMoreElements())
        {
            String key = propKeys.nextElement();
            commandsClassesNames.put(key.toLowerCase(), bundle.getString(key));
            count++;
        }

        logger.info("Successfully read " + count + " properties.");

        try
        {
            propIn.close();
        }
        catch (IOException e)
        {
            logger.fatal("Error closing resource bundle input stream.");
        }
    }

    public Command create(String commandName) throws InvalidCommandException
    {
        logger.trace("Trying to create Command with name '" + commandName + "'.");
        Command instance;

        String className = commandsClassesNames.get(commandName.toLowerCase());
        if (className == null)
        {
            logger.error("Unable to find class name for command name '" + commandName + "'.");
            throw new InvalidCommandException("Unknown command: " + commandName);
        }
        try
        {
            instance = (Command) Class.forName(className).getDeclaredConstructor().newInstance();
        }
        catch (ClassNotFoundException e)
        {
            logger.error("Unable to find class for name '" + className + "'.");
            throw new InvalidCommandException("Unknown command: " + commandName);
        }
        catch (Exception e)
        {
            logger.error("Unable to create new instance of Command with name '" + commandName + "'.\n" + e.getMessage());
            throw new RuntimeException("Error creating Command: " + e.toString());
        }

        logger.info("Successfully created Command instance with name '" + commandName + "'.");

        return instance;
    }
}
