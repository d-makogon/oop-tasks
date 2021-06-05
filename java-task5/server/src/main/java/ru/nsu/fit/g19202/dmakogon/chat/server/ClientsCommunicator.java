package ru.nsu.fit.g19202.dmakogon.chat.server;

import ru.nsu.fit.g19202.dmakogon.chat.Message;

public class ClientsCommunicator
{
    private final ClientsRegistry registry;

    public ClientsCommunicator(ClientsRegistry registry)
    {
        this.registry = registry;
    }

    public void sendToAll(Message message)
    {
        registry.forEach((index, processor) -> processor.receiveMessage(message));
    }

    public void send(ClientProcessor clientProcessor, Message message)
    {
        if (registry.contains(clientProcessor))
        {
            clientProcessor.receiveMessage(message);
        }
    }
}
