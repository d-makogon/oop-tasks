package ru.nsu.fit.g19202.dmakogon.chat.client;

public interface ChatViewObserver
{
    void onUserInput(ChatView view, String input);
}
