package ru.nsu.fit.g19202.dmakogon.chat.server;


import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.fit.g19202.dmakogon.chat.XMLMessageSerializer;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class ChatServer implements AutoCloseable, Runnable
{
    private static final Logger logger = LogManager.getLogger();

    private final ServerSocket serverSocket;
    private final ClientsRegistry registry;
    private final ClientsCommunicator communicator;

    public ChatServer(int port) throws IOException
    {
        serverSocket = new ServerSocket(port);
        registry = new ClientsRegistry();
        communicator = new ClientsCommunicator(registry);
    }

    private void serverLoop() throws IOException
    {
        Socket clientSocket = serverSocket.accept();
        logger.info("Accepted new connection from " + clientSocket.getInetAddress());
        ClientProcessor processor = new ClientProcessor(clientSocket, registry, communicator,
                new XMLMessageSerializer());
        processor.run();
    }

    @Override
    public void run()
    {
        logger.info("Starting chat server at " + serverSocket.getLocalSocketAddress());

        while (true)
        {
            try
            {
                serverLoop();
            }
            catch (IOException e)
            {
                logger.error("Error: " + e.getMessage());
                return;
            }
        }
    }

    @Override
    public void close()
    {
        logger.info("Stopping chat server...");

        registry.forEach((i, client) -> client.stop());

        logger.info("Stopped all clients");
    }
}
