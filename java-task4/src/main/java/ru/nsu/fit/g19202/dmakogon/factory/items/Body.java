package ru.nsu.fit.g19202.dmakogon.factory.items;

import ru.nsu.fit.g19202.dmakogon.factory.IdGenerator;

public class Body implements Item
{
    private final String id = IdGenerator.nextId();

    @Override
    public String getId()
    {
        return id;
    }

    @Override
    public String toString()
    {
        return "Body{" + "id='" + id + '\'' + '}';
    }
}
