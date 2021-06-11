package ru.nsu.fit.g19202.dmakogon.chat.server;

public class ClientInfo
{
    private final int id;
    private final String name;

    public ClientInfo(int id, String name)
    {
        this.id = id;
        this.name = name;
    }

    public int getId()
    {
        return id;
    }

    public String getName()
    {
        return name;
    }

    @Override
    public String toString()
    {
        return "Client (" + "id=" + id + ", name='" + name + '\'' + ')';
    }
}
