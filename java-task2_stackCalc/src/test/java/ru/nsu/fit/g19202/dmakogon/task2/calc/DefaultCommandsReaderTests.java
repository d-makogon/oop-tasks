package ru.nsu.fit.g19202.dmakogon.task2.calc;

import org.apache.logging.log4j.Level;
import org.apache.logging.log4j.core.config.Configurator;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.StackCalculatorException;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class DefaultCommandsReaderTests
{
    @BeforeEach
    void turnOffLogging()
    {
        Configurator.setRootLevel(Level.OFF);
    }

    @Test
    void readsCorrectCommands() throws IOException, StackCalculatorException
    {
        InputStream readerInput = stringToStream("+\npush\nsqrt");
        DefaultCommandsReader commandsReader = new DefaultCommandsReader(readerInput);
        CommandInfo commandInfo;
        int curCommand = 0;
        while ((commandInfo = commandsReader.nextCommand()) != null)
        {
            String cmdName = "add";
            if (curCommand == 1)
            {
                cmdName = "push";
            }
            else if (curCommand == 2)
            {
                cmdName = "sqrt";
            }
            assertEquals(cmdName.toLowerCase(), commandInfo.getCommand().getClass().getSimpleName().toLowerCase());
            curCommand++;
        }
    }

    @Test
    void failsToReadWrongCommand() throws IOException, StackCalculatorException
    {
        InputStream readerInput = stringToStream("cmd");
        DefaultCommandsReader commandsReader = new DefaultCommandsReader(readerInput);
        assertThrows(StackCalculatorException.class, commandsReader::nextCommand);
    }

    private InputStream stringToStream(String s)
    {
        return new ByteArrayInputStream(s.getBytes(StandardCharsets.UTF_8));
    }
}
