package ru.nsu.fit.g19202.dmakogon.chat.server;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.fit.g19202.dmakogon.chat.Message;
import ru.nsu.fit.g19202.dmakogon.chat.MessageSerializer;

import java.io.*;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicBoolean;

public class ClientProcessor implements Runnable
{
    private static final Logger logger = LogManager.getLogger();

    private final Socket clientSocket;
    private final InputStream inputStream;
    private final OutputStream outputStream;

    private final ClientsCommunicator communicator;
    private final ConcurrentLinkedQueue<Message> messageQueue = new ConcurrentLinkedQueue<>();
    private final Object messageQueueLock = new Object();
    private final Object loginLock = new Object();

    private final MessageSerializer messageSerializer;

    private final ClientsRegistry registry;
    private final AtomicBoolean loggedOut = new AtomicBoolean(false);
    private final AtomicBoolean loggedIn = new AtomicBoolean(false);
    private boolean loggedInSuccessfully = false;
    private int id;
    private final Thread outputThread = new Thread(this::outputLoop);
    private ClientInfo clientInfo;
    private final Thread inputThread = new Thread(this::inputLoop);

    public ClientProcessor(Socket clientSocket, ClientsRegistry registry, ClientsCommunicator communicator,
                           MessageSerializer messageSerializer) throws IOException
    {
        this.clientSocket = clientSocket;
        this.communicator = communicator;
        this.registry = registry;
        this.messageSerializer = messageSerializer;
        inputStream = clientSocket.getInputStream();
        outputStream = clientSocket.getOutputStream();
    }

    public ClientInfo getClientInfo()
    {
        return clientInfo;
    }

    public void receiveMessage(Message message)
    {
        // push to msg queue
        messageQueue.add(message);

        synchronized (messageQueueLock)
        {
            messageQueueLock.notifyAll();
        }
    }

    private void inputLoopImpl() throws IOException
    {
        synchronized (loginLock)
        {
            logger.info("Trying to login new client...");

            try
            {
                loggedInSuccessfully = login();
                if (!loggedInSuccessfully)
                {
                    logger.error("Couldn't login new client");
                    return;
                }
            }
            catch (IOException e)
            {
                logger.error("Error: " + e.getMessage());
                loggedInSuccessfully = false;
                return;
            }
            finally
            {
                loggedIn.set(true);
                loginLock.notify();
            }
        }

        while (true)
        {
            Message message = messageSerializer.readMessage(inputStream);
            Message.Type type = message.getType();
            if (type == Message.Type.LIST)
            {
                // user requested list of users

                List<String> clientsNamesList = new ArrayList<>();
                registry.forEach((index, processor) -> clientsNamesList.add(processor.getClientInfo().getName()));

                Message listMessage = Message.createListSuccessMessage(clientsNamesList);
                communicator.send(this, listMessage);

                logger.info(id + " requested list of users");
            }
            else if (type == Message.Type.MESSAGE_OUT)
            {
                // user sent some text

                int clientId = message.getClientId();
                if (id != clientId)
                {
                    Message errorMessage = Message.createErrorMessage("Wrong id");
                    communicator.send(this, errorMessage);
                    // return;
                    continue;
                }

                Message messageToAll = Message.createInTextMessage(clientInfo.getName(), message.getMessage());
                communicator.sendToAll(messageToAll);

                logger.info(id + " sent text message of length " + message.getMessage().length());
            }
            else if (type == Message.Type.LOGOUT)
            {
                // user left the chat

                int clientId = message.getClientId();
                if (id != clientId)
                {
                    Message errorMessage = Message.createErrorMessage("Wrong id");
                    communicator.send(this, errorMessage);
                    continue;
                }

                loggedOut.set(true);

                Message messageToAll = Message.createUserLogoutMessage(clientInfo.getName());
                communicator.sendToAll(messageToAll);

                logger.info(id + " logged out");

                registry.removeClient(id);

                return;
            }
            else
            {
                Message errorMessage = Message.createErrorMessage("Unknown message type");
                communicator.send(this, errorMessage);
                logger.info(id + " sent unknown message type");
            }
        }
    }

    private void outputLoopImpl() throws IOException
    {
        // wait for login from another thread
        synchronized (loginLock)
        {
            while (!loggedIn.get())
            {
                try
                {
                    loginLock.wait();
                }
                catch (InterruptedException e)
                {
                    return;
                }
            }
        }

        if (!loggedInSuccessfully)
        {
            return;
        }

        while (true)
        {
            Message message;
            while ((message = messageQueue.poll()) != null)
            {
                logger.info("Sending message from queue to " + id + "...");
                messageSerializer.writeMessage(outputStream, message);
            }

            if (loggedOut.get())
            {
                return;
            }

            synchronized (messageQueueLock)
            {
                try
                {
                    messageQueueLock.wait();
                }
                catch (InterruptedException e)
                {
                    logger.error("Error: " + e.getMessage());
                    return;
                }
            }
        }
    }

    private void inputLoop()
    {
        try
        {
            inputLoopImpl();
        }
        catch (IOException e)
        {
            logger.error("Error: " + e.getMessage());
        }
    }

    private void outputLoop()
    {
        try
        {
            outputLoopImpl();
        }
        catch (IOException e)
        {
            logger.error("Error: " + e.getMessage());
        }
    }

    public void stop()
    {
        inputThread.interrupt();
        outputThread.interrupt();

        try
        {
            inputThread.join();
            outputThread.join();
            clientSocket.close();
        }
        catch (InterruptedException e)
        {
            logger.error("Error while joining threads: " + e.getMessage());
        }
        catch (IOException e)
        {
            logger.error("Error while closing socket: " + e.getMessage());
        }
    }

    private boolean login() throws IOException
    {
        Message loginMessage = messageSerializer.readMessage(inputStream);

        if (loginMessage.getType() != Message.Type.LOGIN)
        {
            logger.error("User sent not LOGIN message");
            Message errorMessage = Message.createErrorMessage("Expected login message");
            messageSerializer.writeMessage(outputStream, errorMessage);
            return false;
        }

        logger.info("Received LOGIN message from new client");

        String clientName = loginMessage.getClientName();
        if (clientName.length() == 0)
        {
            logger.error("User sent empty name");
            Message errorMessage = Message.createErrorMessage("Name length must be greater than zero");
            messageSerializer.writeMessage(outputStream, errorMessage);
            return false;
        }


        id = registry.addClient(this);
        clientInfo = new ClientInfo(id, clientName);

        logger.info("Successful login for " + clientName + " (id=" + id + ")");

        Message successMessage = Message.createLoginSuccessMessage(id);
        messageSerializer.writeMessage(outputStream, successMessage);

        Message messageToAll = Message.createUserLoginMessage(clientName);
        communicator.sendToAll(messageToAll);

        return true;
    }

    @Override
    public void run()
    {

        inputThread.start();
        outputThread.start();
    }

    public int getId()
    {
        return id;
    }
}
