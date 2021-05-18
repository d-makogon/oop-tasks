package ru.nsu.fit.g19202.dmakogon.factory;

import java.util.concurrent.atomic.AtomicInteger;

public class TestClass
{
    private final AtomicInteger int1;
    private final AtomicInteger int2;

    public TestClass(int int1, int int2)
    {
        this.int1 = new AtomicInteger(int1);
        this.int2 = new AtomicInteger(int2);
    }
}
