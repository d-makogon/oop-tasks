package ru.nsu.fit.g19202.dmakogon.task2.calc;

import java.io.*;
import java.util.HashMap;
import java.util.Map;
import java.util.Stack;

public class CommandExecutionContext implements CommandContext
{
    private final Stack<Double> stack;
    private final Map<String, Double> namedParameters;
    private final Writer writer;

    public CommandExecutionContext()
    {
        this(System.out);
    }

    public CommandExecutionContext(OutputStream outputStream)
    {
        stack = new Stack<>();
        namedParameters = new HashMap<>();
        writer = new OutputStreamWriter(outputStream);
    }

    @Override
    public void push(double value)
    {
        stack.push(value);
    }

    @Override
    public double pop()
    {
        return stack.pop();
    }

    @Override
    public double peek()
    {
        return stack.peek();
    }

    @Override
    public boolean isStackEmpty()
    {
        return stack.isEmpty();
    }

    @Override
    public int getStackSize()
    {
        return stack.size();
    }

    @Override
    public void addNamedParameter(String pName, double pValue)
    {
        namedParameters.put(pName, pValue);
    }

    @Override
    public double getNamedParameter(String pName)
    {
        return namedParameters.get(pName);
    }

    @Override
    public boolean hasNamedParameter(String pName)
    {
        return namedParameters.containsKey(pName);
    }

    @Override
    public Writer getWriter()
    {
        return writer;
    }

    @Override
    public void close() throws IOException
    {
        writer.close();
    }
}
