package ru.nsu.fit.g19202.dmakogon.chat;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public interface MessageSerializer
{
    Message readMessage(InputStream inputStream) throws IOException;

    void writeMessage(OutputStream outputStream, Message message) throws IOException;
}
