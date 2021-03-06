package ru.nsu.fit.g19202.dmakogon.task2.calc;

import java.io.Closeable;
import java.io.Writer;

public interface CommandContext extends Closeable
{
    void push(double value);

    double pop();

    double peek();

    boolean isStackEmpty();

    int getStackSize();

    void addNamedParameter(String name, double value);

    double getNamedParameter(String name);

    boolean hasNamedParameter(String name);

    Writer getWriter();
}
