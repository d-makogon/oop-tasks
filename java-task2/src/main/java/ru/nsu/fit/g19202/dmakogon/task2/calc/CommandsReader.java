package ru.nsu.fit.g19202.dmakogon.task2.calc;

import ru.nsu.fit.g19202.dmakogon.task2.calc.exceptions.InvalidCommandException;

import java.io.Closeable;
import java.io.IOException;

public interface CommandsReader extends Closeable
{
    CommandInfo nextCommand() throws IOException, InvalidCommandException;
}
