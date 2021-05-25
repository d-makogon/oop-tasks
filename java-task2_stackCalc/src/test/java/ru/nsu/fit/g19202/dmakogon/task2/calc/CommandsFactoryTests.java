package ru.nsu.fit.g19202.dmakogon.task2.calc;

import org.apache.logging.log4j.Level;
import org.apache.logging.log4j.core.config.Configurator;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import ru.nsu.fit.g19202.dmakogon.task2.calc.commands.Add;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.CommandNotFoundException;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.InvalidCommandException;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class CommandsFactoryTests
{
    @BeforeEach
    void turnOffLogging()
    {
        Configurator.setRootLevel(Level.OFF);
    }

    @Test
    void doesNotFailWhenEmptyStreamGiven()
    {
        InputStream inputStream = stringToStream("");
        assertDoesNotThrow(() -> new CommandsFactory(inputStream));
    }

    @Test
    void createsExistingCommand() throws CommandNotFoundException, IOException, InvalidCommandException
    {
        String correctCmdName = "ADD";

        InputStream inputStream = stringToStream(correctCmdName + "=" + Add.class.getName());
        CommandsFactory factory = new CommandsFactory(inputStream);
        assertDoesNotThrow(() -> factory.create(correctCmdName));
    }

    @Test
    void failsToCreateWrongCommand() throws CommandNotFoundException, IOException, InvalidCommandException
    {
        String correctCmdName = "ADD";
        String wrongCmdName = "SUB";
        InputStream inputStream = stringToStream(correctCmdName + "=" + Add.class.getName());
        CommandsFactory factory = new CommandsFactory(inputStream);
        assertThrows(CommandNotFoundException.class, () -> factory.create(wrongCmdName));
    }

    private InputStream stringToStream(String s)
    {
        return new ByteArrayInputStream(s.getBytes(StandardCharsets.UTF_8));
    }
}
