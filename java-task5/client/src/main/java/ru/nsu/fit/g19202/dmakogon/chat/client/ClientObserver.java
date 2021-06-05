package ru.nsu.fit.g19202.dmakogon.chat.client;

import ru.nsu.fit.g19202.dmakogon.chat.Message;

public interface ClientObserver
{
    void onLoginSuccess(Client client);

    void onLoginFailure(Client client);

    void onMessageReceive(Client client, Message message);

    void onStop(Client client);
}
