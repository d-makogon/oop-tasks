package ru.nsu.fit.g19202.dmakogon.chat.server;

import java.util.concurrent.ConcurrentHashMap;
import java.util.function.BiConsumer;

public class ClientsRegistry
{
    private final ConcurrentHashMap<Integer, ClientProcessor> clientProcessors = new ConcurrentHashMap<>();
    private int curClientIndex = 0;

    public int addClient(ClientProcessor clientProcessor)
    {
        clientProcessors.put(curClientIndex, clientProcessor);
        return curClientIndex++;
    }

    public void removeClient(int index)
    {
        clientProcessors.remove(index);
    }

    public void forEach(BiConsumer<Integer, ClientProcessor> consumer)
    {
        clientProcessors.forEach(consumer);
    }

    public boolean contains(ClientProcessor processor)
    {
        return clientProcessors.containsKey(processor.getId());
    }
}
