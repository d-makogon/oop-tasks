package ru.nsu.fit.g19202.dmakogon.task2.calc;

import ru.nsu.fit.g19202.dmakogon.task2.calc.commands.Command;

public class CommandInfo
{
    private final Command command;
    private final String[] params;

    public CommandInfo(Command command, String[] params)
    {
        this.command = command;
        this.params = params;
    }

    public Command getCommand()
    {
        return command;
    }

    public String[] getParams()
    {
        return params;
    }
}
