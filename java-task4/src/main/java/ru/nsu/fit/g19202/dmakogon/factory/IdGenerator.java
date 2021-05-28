package ru.nsu.fit.g19202.dmakogon.factory;

import java.util.UUID;

public class IdGenerator
{
    public static String nextId()
    {
        return UUID.randomUUID().toString();
    }
}
